<template>
  <div>
    <div>Message MQTT : {{ mqttMessage }}</div>
    <div>
      <button @click="toggleLight(false)">Éteindre la LED</button>
    </div>
    <div>
      <button @click="toggleLight(true)">Allumer la LED</button>
    </div>
  </div>
</template>

<script>
import mqttClient from "src/mqtt";

export default {
  data() {
    return {
      mqttMessage: "",
    };
  },
  mounted() {
    mqttClient.on("connect", () => {
      console.log("Connecté au serveur MQTT");
      mqttClient.subscribe("device/TempModulIOT8888/temperature");
    });

    mqttClient.on("message", (topic, message) => {
      console.log(`Message reçu: ${message.toString()}`);
      this.mqttMessage = message.toString();
    });

    mqttClient.on("error", (err) => {
      console.error("Erreur de connexion:", err);
    });
  },
  methods: {
    toggleLight(state) {
      const message = state ? "ON" : "OFF";
      console.log("Message try sending");
      mqttClient.publish("device/TempModulIOT8888/light", message);
      console.log("Message send");
    },
  },
  beforeUnmount() {
    mqttClient.end(); // Déconnexion lors de la destruction du composant
  },
};
</script>
