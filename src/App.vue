<template>
  <div class="container">
    <el-header>
      <h1>ESP32 温度气压实时监控系统</h1>
    </el-header>
    
    <el-main>
      <el-row :gutter="20">
        <el-col :span="12">
          <el-card class="data-card">
            <div class="card-title">实时温度</div>
            <div class="value-container">
              <span class="value">{{ currentTemperature }}</span>
              <span class="unit">°C</span>
            </div>
          </el-card>
        </el-col>
        
        <el-col :span="12">
          <el-card class="data-card">
            <div class="card-title">实时气压</div>
            <div class="value-container">
              <span class="value">{{ currentPressure }}</span>
              <span class="unit">hPa</span>
            </div>
          </el-card>
        </el-col>
      </el-row>
      
      <el-row :gutter="20" class="chart-row">
        <el-col :span="24">
          <el-card class="chart-card">
            <div class="card-title">历史数据</div>
            <v-chart class="chart" :option="chartOption" autoresize />
          </el-card>
        </el-col>
      </el-row>
    </el-main>
  </div>
</template>

<script>
import { ref, reactive, onMounted, onUnmounted } from 'vue'
import { use } from 'echarts/core'
import { CanvasRenderer } from 'echarts/renderers'
import { LineChart } from 'echarts/charts'
import { GridComponent, TooltipComponent, TitleComponent, LegendComponent } from 'echarts/components'
import VChart from 'vue-echarts'
import mqtt from 'mqtt/dist/mqtt'
import moment from 'moment'

use([CanvasRenderer, LineChart, GridComponent, TooltipComponent, TitleComponent, LegendComponent])

export default {
  name: 'App',
  components: {
    VChart
  },
  setup() {
    const currentTemperature = ref('--')
    const currentPressure = ref('--')
    let client = null
    
    // MQTT连接配置
    const mqttConfig = {
      // 根据环境选择正确的WebSocket URL
      broker: process.env.NODE_ENV === 'production' 
        ? 'wss://broker.emqx.io:8084/mqtt' // 生产环境使用SSL
        : 'ws://broker.emqx.io:8083/mqtt', // 开发环境
      options: {
        clientId: 'web_client_' + Math.random().toString(16).substring(2, 8),
        clean: true,
        connectTimeout: 4000,
        username: '', // 如果需要
        password: '', // 如果需要
      },
      topics: {
        temperature: 'esp32/temperature111', // 更新为新的topic
        pressure: 'esp32/pressure111'        // 更新为新的topic
      }
    }

    // 图表数据
    const temperatureData = reactive([])
    const pressureData = reactive([])
    const timeData = reactive([])
    
    const chartOption = reactive({
      title: {
        text: '温度和气压变化趋势'
      },
      tooltip: {
        trigger: 'axis'
      },
      legend: {
        data: ['温度(°C)', '气压(hPa)']
      },
      grid: {
        left: '3%',
        right: '4%',
        bottom: '3%',
        containLabel: true
      },
      xAxis: {
        type: 'category',
        boundaryGap: false,
        data: timeData
      },
      yAxis: [
        {
          type: 'value',
          name: '温度(°C)',
          position: 'left'
        },
        {
          type: 'value',
          name: '气压(hPa)',
          min: 900,  // 一般大气压在950-1050之间
          max: 1100,
          position: 'right'
        }
      ],
      series: [
        {
          name: '温度(°C)',
          type: 'line',
          data: temperatureData,
          yAxisIndex: 0,
          smooth: true,
          lineStyle: {
            width: 2,
            color: '#f56c6c'
          },
          areaStyle: {
            opacity: 0.2,
            color: '#f56c6c'
          }
        },
        {
          name: '气压(hPa)',
          type: 'line',
          data: pressureData,
          yAxisIndex: 1,
          smooth: true,
          lineStyle: {
            width: 2,
            color: '#409EFF'
          },
          areaStyle: {
            opacity: 0.2,
            color: '#409EFF'
          }
        }
      ]
    })

    // 用于存储最新数据
    const latestValues = reactive({
      temperature: null,
      pressure: null
    })

    // 连接MQTT服务器
    const connectMqtt = () => {
      try {
        client = mqtt.connect(mqttConfig.broker, mqttConfig.options)
        
        client.on('connect', () => {
          console.log('MQTT 连���成功到 broker.emqx.io')
          // 订阅温度和气压主题
          client.subscribe(mqttConfig.topics.temperature)
          client.subscribe(mqttConfig.topics.pressure)
        })
        
        client.on('message', (topic, message) => {
          const value = parseFloat(message.toString())
          
          // 更新最新值
          if (topic === mqttConfig.topics.temperature) {
            latestValues.temperature = value
            currentTemperature.value = value.toFixed(1)
          } else if (topic === mqttConfig.topics.pressure) {
            latestValues.pressure = value
            currentPressure.value = value.toFixed(1)
          }
          
          // 每次收到消息时，都更新图表
          updateChart()
        })
        
        client.on('error', (err) => {
          console.error('MQTT 连接错误:', err)
        })
      } catch (error) {
        console.error('MQTT 连接初始化错误:', error)
      }
    }
    
    // 更新图表的函数
    const updateChart = () => {
      // 只有当两个传感器的值都有时才更新图表
      if (latestValues.temperature === null || latestValues.pressure === null) {
        return
      }
      
      const currentTime = moment().format('HH:mm:ss')
      
      // 添加新的数据点
      temperatureData.push(latestValues.temperature)
      pressureData.push(latestValues.pressure)
      timeData.push(currentTime)
      
      // 保持数据点数量在合理范围
      if (timeData.length > 20) {
        temperatureData.shift()
        pressureData.shift()
        timeData.shift()
      }
    }

    onMounted(() => {
      connectMqtt()
    })

    onUnmounted(() => {
      if (client) {
        client.end()
      }
    })

    return {
      currentTemperature,
      currentPressure,
      chartOption,
    }
  }
}
</script>

<style>
.container {
  max-width: 1200px;
  margin: 0 auto;
  padding: 20px;
}

.el-header {
  display: flex;
  justify-content: center;
  align-items: center;
  padding: 20px 0;
}

.data-card {
  margin-bottom: 20px;
  text-align: center;
  height: 150px;
}

.chart-card {
  height: 400px;
}

.card-title {
  font-size: 18px;
  color: #606266;
  margin-bottom: 15px;
}

.value-container {
  display: flex;
  justify-content: center;
  align-items: baseline;
}

.value {
  font-size: 48px;
  font-weight: bold;
  color: #303133;
}

.unit {
  font-size: 20px;
  margin-left: 5px;
  color: #606266;
}

.chart {
  height: 320px;
}

.chart-row {
  margin-top: 20px;
}
</style>