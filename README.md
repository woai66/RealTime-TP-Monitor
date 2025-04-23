# ESP32温度气压实时监控系统

一个基于Vue 3和ESP32的实时温度气压监控系统，提供直观的数据可视化和实时监控功能。

## 功能特点

- 实时温度气压数据监控
- 数据可视化展示（使用ECharts）
- 实时数据更新
- 历史数据查询
- 响应式界面设计
- MQTT协议实时通信

## 技术栈

- 前端框架：Vue 3
- UI组件库：Element Plus
- 图表库：ECharts
- 实时通信：MQTT
- 后端服务：Express
- 开发工具：Vue CLI

## 项目结构

```
├── src/                # 源代码目录
├── server/            # 后端服务
├── esp32/            # ESP32相关代码
├── public/           # 静态资源
└── dist/             # 构建输出目录
```

## 安装说明

1. 克隆项目
```bash
git clone [项目地址]
cd temperature-pressure-monitor
```

2. 安装依赖
```bash
npm install
```

3. 启动开发服务器
```bash
# 启动前端开发服务器
npm run serve
# 暂无后端服务
```

4. 构建生产版本
```bash
npm run build
```

## 环境要求

- Node.js >= 14.x
- npm >= 6.x
- ESP32开发板
- MQTT服务器

## 配置说明

1. 创建环境配置文件 `.env`：
```
VUE_APP_MQTT_BROKER=your_mqtt_broker
VUE_APP_MQTT_PORT=your_mqtt_port
VUE_APP_API_URL=your_api_url
```

2. 后端服务配置：
- 在 `server/index.js` 中配置MQTT连接信息
- 配置ESP32设备ID和主题

## 使用说明

1. 确保ESP32设备已正确连接并配置
2. 启动MQTT服务器
3. 运行前端和后端服务
4. 访问 `http://localhost:8080` 查看监控界面

## 开发指南

- 代码规范遵循ESLint配置
- 使用Vue 3组合式API进行开发
- 组件化开发，保持代码可维护性


## 许可证

[MIT License](LICENSE)