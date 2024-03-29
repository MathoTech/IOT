<template>
  <div class="settings-container">
    <h2>Wifi list</h2>
    <div class="button-container" @click="toggleAdd">
      <div>{{ showAdd ? "Retour" : "Ajouter un Wifi" }}</div>
    </div>
    <div class="flex">
      <div class="wifi-list">
        <div
          class="wifi"
          v-for="(wifi, index) in wifiList"
          :key="index"
          :class="{ active: wifi === activeWifi }"
        >
          <div class="wifi-details">
            <div class="wifi-name">{{ wifi.wifi_name }}</div>
            <button @click="toggleConnection(wifi)" class="connect-button">
              {{ wifi === activeWifi ? "Se déconnecter" : "Se connecter" }}
            </button>
            <button @click="removeWifi(index)" class="delete-button">
              Supprimer
            </button>
          </div>
        </div>
      </div>
      <div v-if="showAdd" class="add-container">
        <div class="label">Wifi Name</div>
        <div class="input-container">
          <input v-model="wifi" />
        </div>
        <div class="label">Mot de passe</div>
        <div class="input-container">
          <input v-model="password" type="password" />
        </div>
        <div class="button-container-add" @click="addWifi">
          <div class="input-text">Confirmer</div>
        </div>
      </div>
    </div>
    <h2>Sensor Settings</h2>
    <div class="buttons">
      <div @click="disconnect" class="button-container">Déconnexion Wifi</div>
      <div @click="resetWifiSettings" class="button-container">
        Réinitialiser les paramètres Wi-Fi
      </div>
      <div @click="restart" class="button-container">Restart Sensor</div>
    </div>
    <h2>Sensor Notification</h2>
    <div class="temperature-form">
      <div class="input-container">
        <div class="label">
          Recevoir une notification si la température passe
        </div>
        <select v-model="notificationTemperatureSign">
          <option value="Au dessus">Au dessus</option>
          <option value="En dessous">En dessous</option>
        </select>
      </div>
      <div class="label">A quelle température recevoir notification</div>
      <div class="input-container">
        <input v-model="notificationTemperature" type="number" />
      </div>
      <div class="button-container" @click="saveNotificationTemperature">
        <div class="input-text">Sauvegarder</div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.buttons {
  text-align: center;
  display: flex;
  justify-content: space-between;
}
.wifi.active {
  border: 2px solid #00ff00; /* Bordure verte pour le wifi actif */
}
.connect-button {
  background-color: grey; /* Vert par défaut */
  color: white;
  border: none;
  font-weight: 700;
  padding: 8px 16px;
  border-radius: 5px;
  cursor: pointer;
  transition: background-color 0.3s;
}

.connect-button:hover {
  background-color: #0099ff; /* Légère variation de couleur au survol */
}

.connect-button.disconnect:hover {
  background-color: #cc0000; /* Légère variation de couleur au survol lors de la déconnexion */
}
.button-container-add {
  background-color: inherit;
  overflow: none;
  border-radius: 5px;
  padding: 10px 24px;
  color: #eeeeee;
  transition: 1s;
  margin: auto;
  max-width: 300px;
  background-color: #0099ff;
  cursor: pointer;
}
.button-container:hover {
  background-color: red;
  transition: 1s;
}
.label {
  color: #eeeeee;
  font-size: 16px;
  font-weight: bold;
  padding-bottom: 4px;
}
input {
  background-color: inherit;
  overflow: none;
  border: 1px grey solid;
  border-radius: 5px;
  padding: 10px 24px;
  width: 300px;
  color: #eeeeee;
  transition: 0.5s;
}
input:focus {
  border-color: #0099ff;
  transition: 0.5s;
}
input:hover {
  border-color: #0099ff;
  transition: 0.5s;
}
/* Pour tous les inputs */
input {
  outline: none;
}

/* Si vous voulez cibler spécifiquement les inputs de type texte */
input[type="text"] {
  outline: none;
}

/* Si vous voulez cibler spécifiquement les inputs avec une classe "no-outline" */
.no-outline {
  outline: none;
}
.input-container {
  padding-bottom: 24px;
}
.flex {
  display: flex;
  justify-content: space-between;
}
.add-container {
  width: 45%;
  animation: FadeIn 1s;
  height: 300px;
  border-radius: 5px;
  padding: 24px;
  text-align: center;
}
.button-container {
  margin: auto;
  padding: 8px;
  background-color: grey;
  width: 20%;
  font-weight: 700;
  text-align: center;
  border-radius: 5px;
  margin-bottom: 24px;
  color: #eeeeee;
  cursor: pointer;
}
.settings-container {
  margin: auto;
  width: 80%;
}
.wifi-name {
  color: #eeeeee;
  font-weight: bold;
  font-size: 24px;
}
h2 {
  color: #eeeeee;
  font-size: 36px;
  font-weight: bold;
  text-align: center;
}
.wifi-list {
  row-gap: 40px;
  width: 45%;
}
.wifi {
  border: 1px solid #eeeeee;
  margin-bottom: 16px;
  padding: 16px;
  border-radius: 5px;
}
.wifi-details {
  display: flex;
  justify-content: space-between;
}
.delete-button {
  background-color: #ff3333;
  color: white;
  border: none;
  font-weight: 700;
  padding: 8px 16px;
  border-radius: 5px;
  cursor: pointer;
  transition: background-color 0.3s;
}
.delete-button:hover {
  background-color: #cc0000;
}
</style>

<script>
import { defineComponent } from "vue";
import { firebaseFirestore, firebaseAuth } from "boot/firebase";
import { doc, getDoc, updateDoc, setDoc, deleteDoc } from "firebase/firestore";
import { Notify } from "quasar";
import mqttClient from "src/mqtt";

export default defineComponent({
  name: "SettingsPage",
  data() {
    return {
      wifiList: [{ wifi_name: "Wifi_A" }, { wifi_name: "Wifi_B" }],
      showAdd: false,
      wifi: "",
      password: "",
      activeWifi: null,
      notificationTemperature: undefined,
      notificationTemperatureSign: "Au dessus",
    };
  },
  methods: {
    connectWifi(wifi) {
      this.activeWifi = wifi;
    },
    toggleAdd() {
      this.showAdd = !this.showAdd;
    },
    removeWifi(index) {
      this.wifiList.splice(index, 1);
    },
    toggleConnection(wifi) {
      if (wifi === this.activeWifi) {
        this.activeWifi = null;
      } else {
        this.activeWifi = wifi;
      }
    },
    addWifi() {
      if (this.wifi && this.password) {
        this.wifiList.push({
          wifi_name: this.wifi,
        });

        this.wifi = "";
        this.password = "";

        this.showAdd = false;
      } else {
        alert("Veuillez remplir tous les champs.");
      }
    },

    async saveNotificationTemperature() {
      try {
        const userId = firebaseAuth.currentUser.uid;

        const userRef = doc(firebaseFirestore, "users", userId);

        await updateDoc(userRef, {
          notifTempValue: this.notificationTemperature,
          notifTempSign: this.notificationTemperatureSign,
        });

        Notify.create({
          message: "Température notifiée sauvegardée avec succès",
          color: "positive",
        });
      } catch (error) {
        Notify.create({
          message: "Erreur lors de la sauvegarde de la température notifiée.",
          color: "negative",
        });
        console.error(error);
      }
    },

    async fetchNotifTemp() {
      try {
        const userId = localStorage.getItem("uid");
        const userRef = doc(firebaseFirestore, "users", userId);
        const docSnap = await getDoc(userRef);

        if (docSnap.exists()) {
          this.notificationTemperature = docSnap.data().notifTempValue;
          this.notificationTemperatureSign =
            docSnap.data().notifTempSign == undefined
              ? "Au dessus"
              : docSnap.data().notifTempSign;
        }
      } catch (error) {
        console.error("Erreur lors de la récupération des capteurs :", error);
      }
    },

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

    async disconnect() {
      try {
        const sensors = this.getStoredSensors();
        if (sensors.length > 0) {
          const sensorId = sensors[0];
          if (sensorId === "") throw "Sensors array error";
          mqttClient.publish(`device/${sensorId}/settings`, "DISCONNECT_WIFI");
        }
      } catch (error) {
        console.log(
          "Erreur send cmd disconnect to mqtt topic settings:",
          error
        );
      }
    },

    async resetWifiSettings() {
      try {
        const sensors = this.getStoredSensors();
        if (sensors.length > 0) {
          const sensorId = sensors[0];
          if (sensorId === "") throw "Sensors array error";
          mqttClient.publish(`device/${sensorId}/settings`, "RESET_WIFI");
        }
      } catch (error) {
        console.log("Erreur send cmd reset to mqtt topic settings:", error);
      }
    },

    async restart() {
      try {
        const sensors = this.getStoredSensors();
        if (sensors.length > 0) {
          const sensorId = sensors[0];
          if (sensorId === "") throw "Sensors array error";
          mqttClient.publish(`device/${sensorId}/settings`, "RESTART");
        }
      } catch (error) {
        console.log("Erreur send cmd restart to mqtt topic settings:", error);
      }
    },
  },
  mounted() {
    mqttClient.on("connect", () => {
      console.log("Connecté au serveur MQTT");
    });
    mqttClient.on("error", (err) => {
      console.error("Erreur de connexion:", err);
    });
    this.fetchNotifTemp();
  },
  beforeUnmount() {
    mqttClient.end();
  },
});
</script>
