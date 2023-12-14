<template>
  <q-layout view="lHh Lpr lFf">
    <q-header elevated>
      <q-toolbar>
        <img src="/icons/thermometre.png" />

        <q-toolbar-title class="animated-title">
          <h4>
            <router-link to="/dashboard"
              ><b class="rainbow-text">Thermometer</b></router-link
            >
          </h4>
        </q-toolbar-title>
        <div class="settings-btn">
          <q-btn
            v-if="!isLoginPage && !isSettingsPage"
            icon="settings"
            @click="goToSettings"
          ></q-btn>
        </div>
        <q-btn
          v-if="!isLoginPage"
          icon="power_settings_new"
          @click="logout"
        ></q-btn>
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

export default {
  data() {
    return {
      isHelpModalOpen: false,
    };
  },
  computed: {
    isLoginPage() {
      return this.$route.path === "/login";
    },
    isSettingsPage() {
      return this.$route.path === "/settings";
    },
  },
  components: {
    HelpModal,
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
    logout() {
      // Ajoutez la logique de déconnexion ici
      console.log("Déconnexion");

      // Affiche la notification de déconnexion réussie
      this.$q.notify({
        color: "positive", // Vous pouvez également utiliser 'negative' pour une notification d'erreur
        position: "top",
        message: "Déconnexion réussie",
        icon: "check",
      });

      // Rediriger vers la page de connexion
      this.$router.push("/login");
    },
  },
};
</script>
