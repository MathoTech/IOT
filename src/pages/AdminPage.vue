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
import { defineComponent, onMounted, ref } from "vue";
import { firebaseFirestore } from 'boot/firebase';
import { collection, getDocs, doc, updateDoc, getDoc } from "firebase/firestore";

export default defineComponent({
  name: "AdminPage",

  setup() {
    const users = ref([]);

    onMounted(async () => {
      await fetchUsers();
    });

    async function fetchUsers() {
      try {
        const usersCollection = collection(firebaseFirestore, "users");
        const querySnapshot = await getDocs(usersCollection);
        for (let doc of querySnapshot.docs) {
          let userData = doc.data();
          userData.id = doc.id; // Ajouter l'ID de l'utilisateur
          userData.sensors = await fetchSensors(userData.id); // Récupérer les capteurs de l'utilisateur
          users.value.push(userData);
        }
      } catch (error) {
        console.error("Erreur lors de la récupération des utilisateurs :", error);
      }
    }

    async function fetchSensors(userId) {
      try {
        const sensorRef = doc(firebaseFirestore, "sensors", userId);
        const sensorDoc = await getDoc(sensorRef);
        if (sensorDoc.exists()) {
          return sensorDoc.data().sensorsSerialNumber || [];
        }
        return [];
      } catch (error) {
        console.error("Erreur lors de la récupération des capteurs :", error);
        return [];
      }
    }

    async function removeSensor(userIndex, sensorIndex) {
      const userId = users.value[userIndex].id;
      const updatedSensors = users.value[userIndex].sensors.slice();
      updatedSensors.splice(sensorIndex, 1);

      // Mise à jour du document utilisateur dans Firestore
      const userRef = doc(firebaseFirestore, "sensors", userId);
      await updateDoc(userRef, {
        sensorsSerialNumber: updatedSensors
      });

      // Mise à jour de la liste des utilisateurs
      users.value[userIndex].sensors = updatedSensors;
    }

    function exportToCSV(user) {
      const csvContent =
        "data:text/csv;charset=utf-8," +
        "Id utilisateur,Username,Array des sensors\n" +
        `${user.id},${user.username},"${user.sensors.join('", "')}"`;

      const encodedUri = encodeURI(csvContent);
      const link = document.createElement("a");
      link.setAttribute("href", encodedUri);
      link.setAttribute("download", `${user.username}_sensors.csv`);
      document.body.appendChild(link);
      link.click();
      document.body.removeChild(link);
    }

    return {
      users,
      removeSensor,
      exportToCSV
    };
  },
});
</script>

