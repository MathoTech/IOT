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
    <div class="buttons">
      <button @click="disconnect" class="button-container-add">
        Déconnecter
      </button>
      <button @click="resetWifiSettings" class="button-container-add">
        Réinitialiser les paramètres Wi-Fi
      </button>
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

export default defineComponent({
  name: "SettingsPage",
  data() {
    return {
      wifiList: [
        { wifi_name: "Wifi_A" },
        { wifi_name: "Wifi_B" },
        { wifi_name: "Wifi_C" },
      ],
      showAdd: false,
      wifi: "",
      password: "",
      activeWifi: null, // Wifi actif
    };
  },
  methods: {
    connectWifi(wifi) {
      // Définir le wifi comme actif
      this.activeWifi = wifi;
    },
    toggleAdd() {
      this.showAdd = !this.showAdd;
    },
    removeWifi(index) {
      // Supprime l'élément correspondant à l'index
      this.wifiList.splice(index, 1);
    },
    toggleConnection(wifi) {
      // Toggle entre se connecter et se déconnecter
      if (wifi === this.activeWifi) {
        this.activeWifi = null; // Déconnexion
      } else {
        this.activeWifi = wifi; // Connexion
      }
    },
    addWifi() {
      if (this.wifi && this.password) {
        // Vérifiez que les champs Wifi et Mot de passe ne sont pas vides

        // Ajoute la nouvelle Wifi à la liste
        this.wifiList.push({
          wifi_name: this.wifi,
        });

        // Réinitialise les champs Wifi et Mot de passe
        this.wifi = "";
        this.password = "";

        // Cache l'add-container
        this.showAdd = false;
      } else {
        // Affiche un message d'erreur si les champs sont vides
        alert("Veuillez remplir tous les champs.");
      }
    },
  },
});
</script>
