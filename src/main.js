
import { createApp } from 'vue'
import ElementPlus from 'element-plus'
import 'element-plus/dist/index.css'
import App from './App.vue'
import * as ECharts from 'echarts'

const app = createApp(App)

app.use(ElementPlus)
app.config.globalProperties.$echarts = ECharts
app.mount('#app')