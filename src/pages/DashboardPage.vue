<template>
  <div class="dashboard-container">
    <h2>Dashboard</h2>
    <div class="day-record-container">
      <div class="lowest-container">
        <div class="temp-label">Lowest temp of the day</div>
        <div class="temp">15°C</div>
      </div>
      <div class="actual-container">
        <div class="temp-label">Actual temp</div>
        <div class="temp">23°C</div>
      </div>
      <div class="highest-container">
        <div class="temp-label">Highest temp of the day</div>
        <div class="temp">25°C</div>
      </div>
    </div>
    <h2>Temp Historic</h2>
    <div id="chart" class="chart-container"></div>
  </div>
</template>

<style scoped>
.chart-container {
  margin: auto;
  margin-top: 20px;
  width: 100%;
  max-width: 800px; /* Ajustez la largeur du graphique selon vos besoins */
  height: 300px; /* Ajustez la hauteur du graphique selon vos besoins */
}
.actual-container {
  padding: 12px;
}
.temp {
  color: #eeeeee;
  text-align: center;
  font-weight: 700;
  font-size: 36px;
}
.dashboard-container {
  margin: auto;
  width: 80%;
}
.temp-label {
  color: #eeeeee;
  font-size: 18px;
  font-weight: bold;
  text-align: center;
}
.day-record-container {
  display: flex;
  justify-content: space-between;
  height: 100px;
}
.highest-container {
  padding: 12px;
  width: 40%;
  background-color: #fd1d1d;
  border-radius: 5px;
}
.lowest-container {
  padding: 12px;

  width: 40%;
  background-color: #0099ff;
  border-radius: 5px;
}
h2 {
  color: #eeeeee;
  font-size: 36px;
  font-weight: bold;
  text-align: center;
}
</style>
<script>
import { defineComponent, onMounted, ref, watch, nextTick } from "vue";
import ApexCharts from "apexcharts";

export default defineComponent({
  name: "DashboardPage",

  setup() {
    const isChartReady = ref(false);
    const chartData = ref([20, 15, 18, 17, 25, 23]);

    onMounted(() => {
      // Delay the chart initialization to ensure the DOM element is ready
      nextTick().then(() => {
        initializeChart();
      });
    });

    // Watch for changes in chartData and re-render the chart
    watch(chartData, (newValue) => {
      if (newValue && newValue.length > 0) {
        initializeChart();
      }
    });

    function initializeChart() {
      console.log("Initialize chart");
      try {
        var chartElement = document.querySelector("#chart");
        if (chartElement) {
          console.log("Chart element found");
          var options = {
            chart: {
              type: "line",
            },
            series: [{
              name: "temp",
              data: chartData.value,
            }],
            xaxis: {
              categories: ["00H", "04H", "08H", "12H", "16H", "20H"],
            },
          };

          console.log("Initialize ApexCharts");
          var chart = new ApexCharts(chartElement, options);
          chart.render();
          console.log("Chart rendered");
        } else {
          console.log("Chart element not found");
        }
      } catch (error) {
        console.error("Error initializing chart:", error);
      }
      isChartReady.value = true; // Set this to true for testing
    }
    return {
      isChartReady,
      chartData
    };
  },
});
</script>
