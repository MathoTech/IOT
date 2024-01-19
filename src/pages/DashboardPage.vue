<template>
  <div class="dashboard-container">
    <h2>Dashboard</h2>
    <div class="day-record-container" v-if="sensors.length > 0">
      <div class="lowest-container">
        <div class="temp-label">Lowest temp of the day</div>
        <div class="temp">15°C</div>
      </div>
      <div class="actual-container">
        <div class="temp-label">Actual temp</div>
        <div class="temp">{{ actualTemp }}</div>
      </div>
      <div class="highest-container">
        <div class="temp-label">Highest temp of the day</div>
        <div class="temp">25°C</div>
      </div>
    </div>
    <div v-else class="no-sensor-message">
      <p>Aucun capteur enregistré</p>
      <router-link to="/sensor" class="register-sensor-button"
        >Enregistrer un capteur</router-link
      >
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
.no-sensor-message p {
  color: #eeeeee;
  text-align: center;
  font-size: 20px;
}

.register-sensor-button {
  display: block;
  margin: auto;
  text-align: center;
  background-color: #0099ff;
  color: #ffffff;
  padding: 10px 20px;
  border-radius: 5px;
  text-decoration: none;
  margin-top: 20px;
}

.register-sensor-button:hover {
  background-color: #0077cc;
}
</style>
<script>
import { defineComponent, ref, onMounted } from "vue";
import ApexCharts from "apexcharts";
import mqttClient from "src/mqtt";
import { firebaseFirestore } from "boot/firebase";
import { doc, getDoc, updateDoc, setDoc } from "firebase/firestore";

export default defineComponent({
  name: "DashboardPage",

  data() {
    return {
      chart: null,
      chartData: [],
      sensors: [],
      actualTemp: "N/A", // Température actuelle
    };
  },

  mounted() {
    this.sensors = this.getStoredSensors();
    if (this.sensors.length > 0) {
      this.subscribeToMqtt(this.sensors[0]);
      this.fetchTemperatureData(this.sensors[0]); // Nouvelle méthode pour récupérer les données de température
    }
    this.initializeChart();
  },

  methods: {
    getStoredSensors() {
      const storedSensors = localStorage.getItem("sensors");
      if (storedSensors) {
        if (storedSensors.includes(",")) {
          return storedSensors.split(",");
        } else {
          return [storedSensors];
        }
      } else {
        return [];
      }
    },

    subscribeToMqtt(sensorId) {
      const topic = `device/${sensorId}/temperature`;
      mqttClient.on("connect", () => {
        console.log("Connecté au serveur MQTT");
        mqttClient.subscribe(topic);
        // console.log("Subscribed to topic", topic);
      });
      mqttClient.on("message", (topic, message) => {
        // console.log(`Message reçu sur ${topic}: ${message.toString()}`);
        this.actualTemp = `${message.toString()}°C`;
      });
      mqttClient.on("error", (err) => {
        console.error("Erreur de connexion MQTT:", err);
      });
    },

    initializeChart() {
      const chartOptions = {
        chart: {
          type: "line",
        },
        series: [
          {
            name: "Temperature",
            data: this.chartData,
          },
        ],
        xaxis: {
          categories: ["00H", "04H", "08H", "12H", "16H", "20H"],
        },
      };

      this.chart = new ApexCharts(
        document.querySelector(".chart-container"),
        chartOptions
      );
      this.chart.render();
    },

    fetchTemperatureData(sensorId) {
      const docRef = doc(firebaseFirestore, "savedTemperature", sensorId);
      getDoc(docRef)
        .then((doc) => {
          if (doc.exists) {
            const data = doc.data();
            if (data == undefined || data.readings == undefined) return [];

            // Traitement des données pour le graphique
            this.chartData = data.readings.map(
              (reading) => reading.temperature
            );

            // Traitement des catégories pour l'axe X
            const chartCategories = data.readings.map(
              (reading) => `${reading.dayYear} ${reading.hour}`
            );

            this.updateChart(chartCategories);
          }
        })
        .catch((err) => {
          console.error("Erreur de récupération des données:", err);
        });
    },

    updateChart(chartCategories) {
      if (this.chart) {
        this.chart.updateOptions({
          xaxis: {
            categories: chartCategories,
          },
        });
        this.chart.updateSeries([{ data: this.chartData }]);
      }
    },

    beforeUnmount() {
      if (this.sensors.length > 0) {
        const topic = `device/${this.sensors[0]}/temperature`;
        mqttClient.unsubscribe(topic);
        mqttClient.end();
      }
      if (this.chart) {
        this.chart.destroy();
      }
    },
  },
});
</script>
