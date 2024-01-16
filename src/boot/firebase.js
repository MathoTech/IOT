import { initializeApp } from "firebase/app";
// import { getAnalytics } from "firebase/analytics";
import { getAuth } from 'firebase/auth';
import { getFirestore } from 'firebase/firestore';

const firebaseConfig = {
    apiKey: "AIzaSyBV7LdeuBwy8mpzMzhZBSUkHNxcCNEP1G4",
    authDomain: "temperature-iot-da987.firebaseapp.com",
    projectId: "temperature-iot-da987",
    storageBucket: "temperature-iot-da987.appspot.com",
    messagingSenderId: "1048508454649",
    appId: "1:1048508454649:web:1eee5b31e9fde603a7f428",
    measurementId: "G-VMVEZ2MC9J"
};

const firebaseApp = initializeApp(firebaseConfig);
// const analytics = getAnalytics(firebaseApp);
const firebaseAuth = getAuth(firebaseApp);
const firebaseFirestore = getFirestore(firebaseApp);

export { firebaseApp, firebaseAuth, firebaseFirestore };