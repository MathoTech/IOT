<template>
  <div class="settings-container">
    <h2>Admin Panel</h2>
  </div>
</template>

<style scoped>
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
</style>

<script>
import { defineComponent } from "vue";

export default defineComponent({
  name: "AdminPage",

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
