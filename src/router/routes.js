
const routes = [
  {
    path: '/',
    component: () => import('layouts/MainLayout.vue'),
    children: [
      { path: '/login', component: () => import('src/pages/LandingPage.vue') },
      { path: '/dashboard', component: () => import('src/pages/DashboardPage.vue') },
      { path: '/settings', component: () => import('src/pages/SettingsPage.vue') },
      { path: '/admin', component: () => import('src/pages/AdminPage.vue') },


    ]
  },

  // Always leave this as last one,
  // but you can also remove it
  {
    path: '/:catchAll(.*)*',
    component: () => import('pages/ErrorNotFound.vue')
  }
]

export default routes
