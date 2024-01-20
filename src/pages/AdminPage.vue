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
              <button @click="eraseData(sensor)">Erase Data</button>
              <br />
            </div>
          </td>
          <td>
            <button @click="exportToCSV(user)">Export CSV</button>
            <button @click="deleteUser(user)">Delete</button>
          </td>
        </tr>
      </tbody>
    </table>
    <div @click="exportUsersToCsv" class="button-container">
      Export all users to CSV
    </div>
  </div>
</template>

<style scoped>
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
import { useRouter } from "vue-router";
import { defineComponent, onMounted, ref } from "vue";
import { firebaseAuth, firebaseFirestore } from "boot/firebase";
import {
  collection,
  getDocs,
  doc,
  updateDoc,
  getDoc,
  deleteDoc,
} from "firebase/firestore";
import { Notify } from "quasar";

export default defineComponent({
  name: "AdminPage",

  setup() {
    const users = ref([]);
    const role = ref("user");
    const router = useRouter();

    onMounted(async () => {
      await new Promise((resolve) => setTimeout(resolve, 1000));
      await fetchMe();
      await fetchUsers();
    });

    async function fetchUsers() {
      try {
        const usersCollection = collection(firebaseFirestore, "users");
        const querySnapshot = await getDocs(usersCollection);
        for (let doc of querySnapshot.docs) {
          let userData = doc.data();
          userData.id = doc.id;
          userData.sensors = await fetchSensors(userData.id);
          users.value.push(userData);
        }
      } catch (error) {
        console.error(
          "Erreur lors de la récupération des utilisateurs :",
          error
        );
      }
    }

    async function fetchSensors(userId) {
      try {
        const sensorRef = doc(firebaseFirestore, "users", userId);
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

    async function fetchMe() {
      try {
        const userId = localStorage.getItem("uid");
        const userRef = doc(firebaseFirestore, "users", userId);
        const userDoc = await getDoc(userRef);
        if (userDoc.exists()) {
          role.value = userDoc.data().role;
          if (role.value != "admin") router.push("/dashboard");
        }
      } catch (error) {
        console.error("Erreur lors de la récupération des infos user :", error);
        return "user";
      }
    }

    async function removeSensor(userIndex, sensorIndex) {
      const userId = users.value[userIndex].id;
      const updatedSensors = users.value[userIndex].sensors.slice();
      updatedSensors.splice(sensorIndex, 1);

      // Mise à jour du document utilisateur dans Firestore
      const userRef = doc(firebaseFirestore, "users", userId);
      await updateDoc(userRef, {
        sensorsSerialNumber: updatedSensors,
      });

      // Mise à jour de la liste des utilisateurs
      users.value[userIndex].sensors = updatedSensors;
    }

    async function eraseData(sensor) {
      try {
        const sensorSerialNumber = sensor;

        const sensorDocRef = doc(
          firebaseFirestore,
          "savedTemperature",
          sensorSerialNumber
        );

        // Suppression du document du capteur
        await deleteDoc(sensorDocRef);

        Notify.create({
          message: "Relevés du capteur supprimés avec succès",
          color: "positive",
        });
      } catch (error) {
        Notify.create({
          message: "Erreur lors de la suppression des relevés du capteur.",
          color: "negative",
        });
        console.error(error);
      }
    }

    async function exportToCSV(userData) {
      try {
        const userId = userData.id;
        const userRef = doc(firebaseFirestore, "users", userId);
        const userDoc = await getDoc(userRef);
        if (userDoc.exists()) {
          const userData = userDoc.data();
          const sensorsData = await fetchSensors(userId);
          const sensorsTempData = [];

          if (sensorsData !== undefined && sensorsData.length > 0) {
            for (let i = 0; i < sensorsData.length; i++) {
              const sensorId = sensorsData[i];
              const readings = await fetchTemp(sensorId);
              sensorsTempData.push({ sensorId, readings });
            }

            let csvContent = "Sensor ID;Date;Temperature\n";
            sensorsTempData.forEach((sensorData) => {
              const sensorId = sensorData.sensorId;
              for (let index = 0; index < sensorData.readings.length; index++) {
                const element = sensorData.readings[index];
                console.log(element);
                const date = `${element.dayYear} ${element.hour}`;
                const temperature = element.temperature;
                csvContent += `${sensorId};${date};${temperature}\n`;
              }
            });

            const blob = new Blob([csvContent], {
              type: "text/csv;charset=utf-8",
            });

            const url = window.URL.createObjectURL(blob);

            const a = document.createElement("a");
            a.href = url;
            a.download = `sensor_data_${userId}.csv`;

            a.click();

            window.URL.revokeObjectURL(url);
          }
        }
      } catch (error) {
        console.error(
          "Erreur lors de la génération et du téléchargement du fichier CSV :",
          error
        );
      }
    }

    async function exportUsersToCsv() {
      try {
        const querySnapshot = await getDocs(
          collection(firebaseFirestore, "users")
        );
        let csvContent =
          "User ID;Email;Last Notified;Notification Temp Sign;Notification Temp Value;Role;Sensors Serial Number;Username\n";

        querySnapshot.forEach((doc) => {
          const userData = doc.data();
          const userId = doc.id;
          const email = userData.email || "";
          const lastNotified = userData.lastNotified
            ? new Date(userData.lastNotified.seconds * 1000).toLocaleString()
            : "";
          const notifTempSign = userData.notifTempSign || "";
          const notifTempValue = userData.notifTempValue || "";
          const role = userData.role || "";
          const sensorsSerialNumber = userData.sensorsSerialNumber
            ? userData.sensorsSerialNumber.join(", ")
            : "";
          const username = userData.username || "";

          csvContent += `${userId};${email};${lastNotified};${notifTempSign};${notifTempValue};${role};${sensorsSerialNumber};${username}\n`;
        });

        const blob = new Blob([csvContent], {
          type: "text/csv;charset=utf-8",
        });

        const url = window.URL.createObjectURL(blob);

        const a = document.createElement("a");
        a.href = url;
        a.download = "all_users_data.csv";

        a.click();

        window.URL.revokeObjectURL(url);
      } catch (error) {
        console.error(
          "Erreur lors de la génération et du téléchargement du fichier CSV pour tous les utilisateurs :",
          error
        );
      }
    }

    async function fetchTemp(sensorId) {
      const docRef = doc(firebaseFirestore, "savedTemperature", sensorId);
      try {
        const doc = await getDoc(docRef);
        if (doc.exists) {
          const data = doc.data();
          if (data == undefined || data.readings == undefined) return [];
          console.log(data.readings);
          return data.readings;
        }
      } catch (err) {
        console.error("Erreur de récupération des données:", err);
        return [];
      }
    }

    async function deleteUser(user) {
      try {
        const userRef = doc(firebaseFirestore, "users", user.id);
        await deleteDoc(userRef);

        users.value = users.value.filter((u) => u.id !== user.id);

        Notify.create({
          message: `Utilisateur ${user.username} supprimé avec succès`,
          color: "positive",
        });
      } catch (error) {
        console.error(
          "Erreur lors de la suppression de l'utilisateur :",
          error
        );
        Notify.create({
          message: "Erreur lors de la suppression de l'utilisateur.",
          color: "negative",
        });
      }
    }

    return {
      users,
      removeSensor,
      eraseData,
      exportToCSV,
      exportUsersToCsv,
      deleteUser,
      role,
    };
  },
});
</script>
