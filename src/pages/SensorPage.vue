<template>
  <div class="landing-container">
    <div class="sensor-container">
      <h2>Ajouter un Capteur</h2>

      <div class="label">Numéro de Série</div>
      <div class="input-container">
        <input v-model="serialNumber" />
      </div>

      <div class="button-container" @click="registerSensor">
        <div class="input-text">Enregistrer le Capteur</div>
      </div>

      <!-- Affichage de la liste des capteurs enregistrés -->
      <div v-if="sensors.length > 0" class="sensor-list">
        <h2 v-if="sensors.length > 1">Capteurs Enregistrés</h2>
        <h2 v-if="sensors.length <= 1">Capteurs Enregistrés</h2>
        <div class="card" v-for="(sn, index) in sensors" :key="index">
          <div class="card-content">
            {{ sn }}
            <span class="delete-icon" @click="deleteSensor(index)"> 🗑️ </span>
            <button class="dashboard-button" @click="redirectToDashboard">
              Look Data
            </button>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { firebaseAuth, firebaseFirestore } from "boot/firebase";
import { defineComponent, onMounted, ref } from "vue";
import { Notify } from "quasar";
import { doc, getDoc, updateDoc, setDoc } from "firebase/firestore";

const MAX_SENSOR = 2;

export default defineComponent({
  name: "AddSensorPage",
  data() {
    return {
      serialNumber: "",
      sensors: [], // Liste des numéros de série des capteurs
    };
  },
  methods: {
    redirectToDashboard() {
      this.$router.push("/dashboard");
    },
    async registerSensor() {
      try {
        if (this.serialNumber === "") throw "SerialNumber empty";
        // Obtenir l'ID de l'utilisateur connecté
        const userId = firebaseAuth.currentUser.uid;

        // Référencer le document de l'utilisateur dans la collection 'sensors'
        const userRef = doc(firebaseFirestore, "sensors", userId);

        // Vérifier si le document existe
        const docSnap = await getDoc(userRef);

        if (docSnap.exists()) {
          // Mettre à jour le document existant
          if (docSnap.data().sensorsSerialNumber.length >= MAX_SENSOR) {
            Notify.create({
              message:
                "Nombre de sensor max: " +
                MAX_SENSOR +
                " par utilisateur / Erreur lors de l'enregistrement du capteur.",
              color: "negative",
            });
            return;
          }
          await updateDoc(userRef, {
            sensorsSerialNumber: [
              ...docSnap.data().sensorsSerialNumber,
              this.serialNumber,
            ],
          });
          localStorage.setItem("sensors", [
            ...docSnap.data().sensorsSerialNumber,
            this.serialNumber,
          ]);
        } else {
          // Créer un nouveau document
          await setDoc(userRef, {
            sensorsSerialNumber: [this.serialNumber],
          });
          localStorage.setItem("sensors", [this.serialNumber]);
        }

        Notify.create({
          message: "Capteur enregistré avec succès",
          color: "positive",
        });

        // Redirection après l'enregistrement
        this.$router.push("/dashboard");
      } catch (error) {
        Notify.create({
          message: "Erreur lors de l'enregistrement du capteur.",
          color: "negative",
        });
        console.error(error);
      }
    },

    async fetchSensors() {
      try {
        const userId = localStorage.getItem("uid");
        const userRef = doc(firebaseFirestore, "sensors", userId);
        const docSnap = await getDoc(userRef);

        if (docSnap.exists()) {
          this.sensors = docSnap.data().sensorsSerialNumber;
          localStorage.setItem("sensors", docSnap.data().sensorsSerialNumber);
        }
      } catch (error) {
        console.error("Erreur lors de la récupération des capteurs :", error);
      }
    },

    async deleteSensor(index) {
      try {
        const userId = localStorage.getItem("uid");

        // Référencer le document de l'utilisateur dans la collection 'sensors'
        const userRef = doc(firebaseFirestore, "sensors", userId);

        // Vérifier si le document existe
        const docSnap = await getDoc(userRef);

        if (docSnap.exists()) {
          // Créer un nouveau tableau sans l'élément à supprimer
          let updatedSensors = [...docSnap.data().sensorsSerialNumber];
          updatedSensors.splice(index, 1);

          // Mettre à jour le document avec le nouveau tableau
          await updateDoc(userRef, {
            sensorsSerialNumber: updatedSensors,
          });

          // Mettre à jour la liste des capteurs dans l'interface utilisateur
          this.sensors = updatedSensors;
          localStorage.setItem("sensors", updatedSensors);

          Notify.create({
            message: "Capteur supprimé avec succès",
            color: "positive",
          });
        }
      } catch (error) {
        Notify.create({
          message: "Erreur lors de la suppression du capteur.",
          color: "negative",
        });
        console.error(error);
      }
    },
  },
  mounted() {
    this.fetchSensors(); // Récupérer la liste des capteurs lors du montage du composant
  },
});
</script>


<style scoped>
.dashboard-button {
  background-color: #0099ff; /* Couleur de fond du bouton Dashboard */
  color: #ffffff; /* Couleur du texte du bouton Dashboard */
  border: none;
  padding: 4px 8px;
  border-radius: 5px;
  cursor: pointer;
  transition: background-color 0.3s;
}
/* Styles adaptés pour la nouvelle classe .sensor-container */
.sensor-container {
  margin: auto;
  animation: SlideFromBottom 1s;
}
.input-text {
  font-weight: bold;
}
.button-container {
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
.input-container {
  padding-bottom: 24px;
}
.landing-container {
  width: 40%;
  margin: auto;
  padding-top: 150px;
  text-align: center;
}
h2 {
  color: #eeeeee;
  font-size: 30px;
  font-weight: bold;
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
.sensor-list {
  margin-top: 20px;
}

.card {
  background-color: #2c3e50;
  border-radius: 10px;
  padding: 15px;
  margin-bottom: 10px;
  color: #eeeeee;
  box-shadow: 0 2px 5px rgba(0, 0, 0, 0.2);
}

.card-content {
  font-size: 16px;
}

h2 {
  color: #eeeeee;
  font-size: 30px;
  font-weight: bold;
  margin-bottom: 20px; /* Ajout d'une marge en bas du titre */
}

/* Styles supplémentaires pour le conteneur du formulaire */
.form-container {
  margin-bottom: 40px; /* Espacement entre le formulaire et la liste des capteurs */
}
.card-content {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.delete-icon {
  cursor: pointer;
  padding: 5px;
  font-size: 1.2em; /* Ajustez la taille de l'icône si nécessaire */
}
</style>
