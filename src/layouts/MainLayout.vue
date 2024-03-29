<template>
  <q-layout view="lHh Lpr lFf">
    <q-header elevated>
      <q-toolbar>
        <img src="../assets/thermometre.png" />

        <q-toolbar-title class="animated-title">
          <h4 v-if="isLoginPage || isAdminPage">
            <b class="rainbow-text">Thermometer</b>
          </h4>
          <h4 v-if="!isLoginPage && !isAdminPage">
            <router-link to="/dashboard"
              ><b class="rainbow-text">Thermometer</b></router-link
            >
          </h4>
        </q-toolbar-title>
        <div class="links">
          <router-link
            class="nav-link"
            v-if="!isLoginPage && !isAdminPage"
            to="/dashboard"
            >Dashboard</router-link
          >
          <router-link
            class="nav-link"
            v-if="!isLoginPage && !isAdminPage"
            to="/settings"
            >Settings</router-link
          >
          <router-link
            class="nav-link"
            v-if="!isLoginPage && !isAdminPage"
            to="/sensor"
            >Sensor</router-link
          >
        </div>
        <q-btn
          v-if="!isLoginPage"
          icon="power_settings_new"
          @click="logout"
        ></q-btn>
        <!-- Ajoutez des liens vers toutes les pages ici -->
      </q-toolbar>
    </q-header>
    <q-page-container :class="{ 'blur-background': isHelpModalOpen }">
      <router-view />
    </q-page-container>
    <div class="help-container" @click="openHelpModal">
      <q-icon name="help" color="white" size="24px" />
    </div>
    <HelpModal ref="helpModal" @modal-closed="closeHelpModal" />
  </q-layout>
</template>

<style scoped>
.links {
  display: flex;
  color: #ffffff; /* Couleur du texte en blanc */
  justify-content: space-around;
  margin-right: 24px; /* Espacement à droite */
}

.nav-link {
  text-decoration: none !important;
  color: #ffffff; /* Couleur du texte en blanc */
  padding: 8px; /* Espacement autour des liens */
  cursor: pointer;
}
a {
  font-weight: bold;
  color: #eeeeee !important;
  text-decoration: none !important;
  padding: 8px; /* Espacement autour des liens */
  cursor: pointer;
}
.settings-btn {
  margin-right: 24px;
}
.white {
  color: #ffffff !important;
  text-decoration: none !important;
}
.footer-image {
  text-align: center;
}

.header-icons {
  text-align: center;
  color: #eeeeee;
  font-size: 24px;
  padding-bottom: 16px;
}

.icons {
  margin: auto;
  display: flex;
  justify-content: space-around;
  width: 50%;
  align-items: center;
}
.icon-container {
  padding: 8px;
  text-decoration: underline;
  border-radius: 5px;
  transition: 0.5s;
}
.icon-container:hover {
  background-color: #404040;
  transition: 0.5s;
}

i {
  font-size: 24px;
}

.padding-right  {
  margin-right: 24px;
}

a {
  cursor: pointer;
}
.footer-content {
  align-items: center;
  justify-content: space-between;
  color: #eeeeee;
  margin: 16px;
}
.footer-line {
  opacity: 0.1;
}
footer {
  padding: 8px;
}
:-webkit-any-link {
  color: initial;
  text-decoration: none;
}
.blur-background {
  filter: blur(5px);
  pointer-events: none;
  transition: filter 0.3s ease-in-out;
}

.help-container {
  border-radius: 5px;
  border: 1px solid #eeeeee;
  padding: 8px;
  margin-right: 8px;
  cursor: pointer;
  position: fixed;
  bottom: 20px;
  right: 20px;
  z-index: 1000;
  opacity: 0.5;
  transition: 0.5s;
}

.help-container:hover {
  opacity: 1;
  transition: 0.5s;
}

img {
  height: 40px;
  width: auto;
}

.q-layout {
  background-color: #191919;
}

.q-header {
  box-shadow: 0px 4px 6px rgba(0, 0, 0, 0.8);
  background-color: #191919;
}

.q-toolbar-title {
  padding-left: 0px !important;
  font-size: 24px;
  display: flex;
  align-items: center;
}

.title-text {
  margin-right: 8px;
}

span {
  color: #eeeeee;
}

.q-toolbar {
  height: 80px;
}

router-link {
  text-decoration: none !important;
}

h4 {
  text-decoration: none !important;
  cursor: pointer;
  animation: slideFromTop 1.2s, FadeIn 1.2s !important;
}
</style>

<script>
import HelpModal from "../components/HelpModal.vue";
import { firebaseAuth } from "boot/firebase";
import { onAuthStateChanged, signOut } from "firebase/auth";
import { defineComponent } from "vue";

export default defineComponent({
  name: "MainLayout",
  data() {
    return {
      isHelpModalOpen: false,
    };
  },
  computed: {
    isAdminPage() {
      return this.$route.path === "/admin";
    },
    isLoginPage() {
      return this.$route.path === "/" || this.$route.path === "/register";
    },
    isSettingsPage() {
      return this.$route.path === "/settings";
    },
  },
  components: {
    HelpModal,
  },
  created() {
    onAuthStateChanged(firebaseAuth, (user) => {
      if (user) {
        const uid = user.uid;
        console.log("Utilisateur connecté avec l'ID :", uid);
        localStorage.setItem("uid", uid);
      } else {
        console.log("Utilisateur déconnecté");
        if (!this.isLoginPage) {
          this.$router.push("/");
        }
      }
    });
  },
  methods: {
    goToSettings() {
      this.$router.push("/settings");
    },
    openHelpModal() {
      this.$refs.helpModal.openModal();
      this.isHelpModalOpen = true;
    },
    closeHelpModal() {
      this.isHelpModalOpen = false;
    },
    async logout() {
      try {
        await signOut(firebaseAuth);

        this.$q.notify({
          color: "positive",
          position: "top",
          message: "Déconnexion réussie",
          icon: "check",
        });

        localStorage.removeItem("uid");
        localStorage.removeItem("sensors");

        this.$router.push("/");
      } catch (error) {
        console.error(error);
        this.$q.notify({
          color: "negative",
          position: "top",
          message: "Erreur lors de la déconnexion. Veuillez réessayer.",
        });
      }
    },
  },
});
</script>
