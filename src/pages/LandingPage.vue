<template>
  <div class="landing-container">
    <div class="login-container">
      <h2>Connexion</h2>

      <div class="label">Identifiant</div>
      <div class="input-container">
        <input v-model="id" />
      </div>
      <div class="label">Mot de passe</div>
      <div class="input-container">
        <input v-model="password" type="password" />
      </div>
      <div class="button-container" @click="attemptLogin">
        <div class="input-text">Se connecter</div>
      </div>
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
</style>

<script>
import { defineComponent } from "vue";
import { Notify } from "quasar";

export default defineComponent({
  name: "LandingPage",
  data() {
    return {
      id: "",
      password: "",
    };
  },
  methods: {
    attemptLogin() {
      // Vérifiez les identifiants ici
      if (
        (this.id === "root" && this.password === "root") ||
        (this.id === "admin" && this.password === "admin")
      ) {
        // Connexion réussie
        Notify.create({
          message: "Connexion réussie",
          color: "positive",
        });
        setTimeout(() => {
          // Redirection en fonction du rôle
          if (this.id === "admin") {
            this.$router.push("/admin");
          } else {
            this.$router.push("/dashboard");
          }
        }, 1000);
      } else {
        // Erreur de connexion
        Notify.create({
          message: "Erreur de connexion. Veuillez vérifier vos identifiants.",
          color: "negative",
        });
      }
    },
  },
});
</script>
