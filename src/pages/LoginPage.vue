<template>
  <div class="landing-container">
    <div class="login-container">
      <h2>Connexion</h2>

      <div class="label">Identifiant</div>
      <div class="input-container">
        <input v-model="email" />
      </div>
      <div class="label">Mot de passe</div>
      <div class="input-container">
        <input v-model="password" type="password" />
      </div>
      <div class="button-container" @click="attemptLogin">
        <div class="input-text">Se connecter</div>
      </div>
      <router-link to="/register" class="register-link">S'inscrire</router-link>
    </div>
  </div>
</template>

<style scoped>
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
.login-container {
  margin: auto;
  animation: SlideFromBottom 1s;
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
.register-link {
  color: #0099ff; /* Couleur du lien */
  text-decoration: underline; /* Soulignez le lien */
  cursor: pointer;
  margin-top: 8px; /* Espacement du lien par rapport aux autres éléments */
  display: block; /* Afficher en tant que bloc pour occuper la largeur disponible */
}

.register-link:hover {
  color: red; /* Couleur du lien au survol */
  transition: 0.5s;
}
</style>

<script>
import { signInWithEmailAndPassword } from "firebase/auth";
import { firebaseAuth, firebaseFirestore } from "boot/firebase";
import { defineComponent, onMounted, ref } from "vue";
import { Notify } from "quasar";
import { doc, getDoc, updateDoc, setDoc, deleteDoc } from "firebase/firestore";

export default defineComponent({
  name: "LandingPage",
  data() {
    return {
      id: "",
      password: "",
    };
  },
  methods: {
    async attemptLogin() {
      try {
        const userCredential = await signInWithEmailAndPassword(
          firebaseAuth,
          this.email,
          this.password
        );

        const user = userCredential.user;
        Notify.create({
          message: "Connexion réussie",
          color: "positive",
        });
        if (user) {
          try {
            const userRef = doc(
              firebaseFirestore,
              "users",
              userCredential.user.uid
            );
            const docSnap = await getDoc(userRef);

            if (docSnap.exists()) {
              if (docSnap.data().role == "admin") {
                setTimeout(() => {
                  this.$router.push("/admin");
                }, 1000);
                return;
              }
              this.sensors = docSnap.data().sensorsSerialNumber;
              localStorage.setItem(
                "sensors",
                docSnap.data().sensorsSerialNumber
              );
            }
          } catch (error) {
            console.error(
              "Erreur lors de la récupération des capteurs :",
              error
            );
          }
          setTimeout(() => {
            this.$router.push("/dashboard");
          }, 1000);
        }
      } catch (error) {
        Notify.create({
          message: "Erreur de connexion. Veuillez vérifier vos identifiants.",
          color: "negative",
        });
        console.error(error);
      }
    },
  },
});
</script>
