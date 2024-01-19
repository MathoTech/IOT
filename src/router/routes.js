const routes = [
  {
    path: "/",
    component: () => import("layouts/MainLayout.vue"),
    children: [
      { path: "/", component: () => import("src/pages/LoginPage.vue") },
      {
        path: "/register",
        component: () => import("src/pages/RegisterPage.vue"),
      },
      {
        path: "/dashboard",
        component: () => import("src/pages/DashboardPage.vue"),
      },
      {
        path: "/settings",
        component: () => import("src/pages/SettingsPage.vue"),
      },
      { path: "/admin", component: () => import("src/pages/AdminPage.vue") },
      { path: "/sensor", component: () => import("src/pages/SensorPage.vue") },
    ],
  },

  // Always leave this as last one,
  // but you can also remove it
  {
    path: "/:catchAll(.*)*",
    component: () => import("pages/ErrorNotFound.vue"),
  },
];

export default routes;
