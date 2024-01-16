<template>
  <div>Message MQTT : {{ mqttMessage }}</div>
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
      mqttClient.subscribe("device/1/temperature");
    });

    mqttClient.on("message", (topic, message) => {
      console.log(`Message reçu: ${message.toString()}`);
      this.mqttMessage = message.toString();
    });

    mqttClient.on("error", (err) => {
      console.error("Erreur de connexion:", err);
    });
  },
  beforeUnmount() {
    mqttClient.end(); // Déconnexion lors de la destruction du composant
  },
};
</script>
