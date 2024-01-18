<template>
  <div class="settings-container">
    <h2>Admin Panel</h2>

    <table>
      <thead>
        <tr>
          <th>Id utilisateur</th>
          <th>Username</th>
          <th>Array des sensors</th>
          <th>Actions</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="(user, index) in users" :key="index">
          <td>{{ user.id }}</td>
          <td>{{ user.username }}</td>
          <td>
            <div
              v-for="(sensor, sensorIndex) in user.sensors"
              :key="sensorIndex"
            >
              {{ sensor }}
              <button @click="removeSensor(index, sensorIndex)">Delete</button>
              <br />
            </div>
          </td>
          <td>
            <button @click="exportToCSV(user)">Export CSV</button>
          </td>
        </tr>
      </tbody>
    </table>
  </div>
</template>

<style scoped>
button {
  margin-left: 4px;
  padding: 4px 8px;
  border-radius: 5px;
}
.settings-container {
  margin: auto;
  width: 80%;
}

h2 {
  color: #eeeeee;
  font-size: 36px;
  font-weight: bold;
  text-align: center;
}

table {
  width: 100%;
  border-collapse: collapse;
  margin-top: 20px;
}

th,
td {
  border: 1px solid #dddddd;
  text-align: left;
  color: #eeeeee;
  padding: 8px;
}

th {
  text-align: center;
  background-color: orange;
  color: white;
}
</style>

<script>
import { defineComponent } from "vue";

export default defineComponent({
  name: "AdminPage",

  data() {
    return {
      users: [
        { id: 1, username: "Matho", sensors: ["sensor1", "sensor2"] },
        { id: 2, username: "Pierre", sensors: ["sensor3", "sensor4"] },
        // ... Ajoutez d'autres utilisateurs avec leurs données
      ],
    };
  },

  methods: {
    removeSensor(userIndex, sensorIndex) {
      this.users[userIndex].sensors.splice(sensorIndex, 1);
    },

    exportToCSV(user) {
      const csvContent =
        "data:text/csv;charset=utf-8," +
        "Id utilisateur,Username,Array des sensors\n" +
        `${user.id},${user.username},"${user.sensors.join('", "')}"`;

      const encodedUri = encodeURI(csvContent);
      const link = document.createElement("a");
      link.setAttribute("href", encodedUri);
      link.setAttribute("download", `${user.username}_sensors.csv`);
      document.body.appendChild(link); // Required for FF
      link.click();
      document.body.removeChild(link);
    },
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
