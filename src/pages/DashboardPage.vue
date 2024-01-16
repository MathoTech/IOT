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
    <div ref="chart" class="chart-container"></div>
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
import { defineComponent, onMounted, ref, watch } from "vue";
import ApexCharts from "apexcharts";

export default defineComponent({
  name: "DashboardPage",

  setup() {
    const chart = ref(null);
    const chartData = ref([20, 15, 18, 17, 25, 23]);

    onMounted(() => {
      // Delay ApexCharts initialization by 2 seconds
      setTimeout(() => {
        initializeChart();
      }, 2000);
    });

    // Watch for changes in chartData and re-render the chart
    watch(chartData, () => {
      if (chart.value) {
        updateChart();
      }
    });

    function initializeChart() {
      try {
        const chartOptions = {
          chart: {
            type: "line",
          },
          series: [
            {
              name: "temp",
              data: chartData.value,
            },
          ],
          xaxis: {
            categories: ["00H", "04H", "08H", "12H", "16H", "20H"],
          },
        };

        chart.value = new ApexCharts(
          document.querySelector(".chart-container"),
          chartOptions
        );
        chart.value.render();
      } catch (error) {
        console.error("Error initializing chart:", error);
      }
    }

    function updateChart() {
      try {
        if (chart.value) {
          chart.value.updateSeries([
            {
              data: chartData.value,
            },
          ]);
        }
      } catch (error) {
        console.error("Error updating chart:", error);
      }
    }

    return {
      chart,
      chartData,
    };
  },
});
</script>
