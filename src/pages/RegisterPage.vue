<template>
  <div class="landing-container">
    <div class="login-container">
      <h2>Créer un compte</h2>

      <div class="label">Email</div>
      <div class="input-container">
        <input v-model="email" type="email" />
      </div>
      <div class="label">Username</div>
      <div class="input-container">
        <input v-model="username" type="username" />
      </div>
      <div class="label">Mot de passe</div>
      <div class="input-container">
        <input v-model="password" type="password" />
      </div>
      <div class="button-container" @click="register">
        <div class="input-text">S'inscrire</div>
      </div>
      <router-link to="/" class="register-link">Se connecter</router-link>
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
import { firebaseAuth, firebaseFirestore } from "boot/firebase";
import { createUserWithEmailAndPassword } from "firebase/auth";
import { setDoc, doc } from "firebase/firestore";
import { defineComponent } from "vue";
import { Notify } from "quasar";

export default defineComponent({
  name: "RegisterPage",
  data() {
    return {
      email: "",
      password: "",
      username: "",
    };
  },
  methods: {
    async register() {
      try {
        const userCredential = await createUserWithEmailAndPassword(
          firebaseAuth,
          this.email,
          this.password
        );

        const user = userCredential.user;

        const userRef = doc(firebaseFirestore, "users", user.uid);

        await setDoc(userRef, {
          username: this.username,
          email: this.email,
          sensorsSerialNumber: [],
          notifTempValue: null,
          role: "user",
        });

        this.$router.push("/dashboard");

        this.$q.notify({
          message:
            "Compte créé avec succès. Vous pouvez maintenant vous connecter.",
          color: "positive",
        });
      } catch (error) {
        console.log(error);
        this.$q.notify({
          message: "Erreur lors de la création du compte. Veuillez réessayer.",
          color: "negative",
        });
      }
    },
  },
});
</script>
