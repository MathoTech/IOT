import mqtt from "mqtt";

const clientId = `mqtt_${Math.random().toString(16).substr(2, 8)}`;
const options = {
  connectTimeout: 4000,
  clientId,
  keepalive: 60,
  clean: true,
  username: "admin",
  password: "CRM6aqZbmRiCZYTiGA8CYGoIlV6xBL4j",
  reconnectPeriod: 1000,
};

const url = "wss://u9v0uc.stackhero-network.com/";

const client = mqtt.connect(url, options);

export default client;
