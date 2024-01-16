<script>
import { defineComponent, onMounted } from "vue";
import ApexCharts from "apexcharts";

export default defineComponent({
  name: "DashboardPage",

  setup() {
    onMounted(() => {
      var options = {
        chart: {
          type: "line",
        },
        series: [
          {
            name: "temp",
            data: [20, 15, 18, 17, 25, 23],
          },
        ],
        xaxis: {
          categories: ["00H", "04H", "08H", "12H", "16H", "20H"],
        },
      };

      var chart = new ApexCharts(document.querySelector("#chart"), options);

      chart.render();
    });

    return {};
  },

  beforeRouteEnter(to, from, next) {
    const uid = localStorage.getItem("uid");

    if (!uid) {
      // Rediriger vers la page de connexion
      next("/");
    } else {
      next();
    }
  },
});
</script>
