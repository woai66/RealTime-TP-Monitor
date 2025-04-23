const express = require('express');
const cors = require('cors');
const mqtt = require('mqtt');
const app = express();
const port = 3000;

// 允许跨域
app.use(cors());
app.use(express.json());

// 存储最新数据
let latestData = {
  temperature: null,
  pressure: null,
  lastUpdated: null
};

// 连接MQTT代理
const client = mqtt.connect('mqtt://broker.emqx.io:1883', {
  clientId: 'node_server_' + Math.random().toString(16).substring(2, 8)
});

// 订阅主题
client.on('connect', () => {
  console.log('已连接到MQTT代理服务器');
  client.subscribe('esp32/temperature11'); // 更新为新的topic
  client.subscribe('esp32/pressure11');    // 更新为新的topic
});

// 处理接收到的消息
client.on('message', (topic, message) => {
  const value = parseFloat(message.toString());
  
  if (topic === 'esp32/temperature11') {  // 更新为新的topic
    latestData.temperature = value;
  } else if (topic === 'esp32/pressure11') {  // 更新为新的topic
    latestData.pressure = value;
  }
  
  latestData.lastUpdated = new Date();
});

// API端点获取最新数据
app.get('/api/latest-data', (req, res) => {
  res.json(latestData);
});

// 启动服务器
app.listen(port, () => {
  console.log(`服务器运行在 http://localhost:${port}`);
});