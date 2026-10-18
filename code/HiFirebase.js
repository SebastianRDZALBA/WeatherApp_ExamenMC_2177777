
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
const firebaseConfig = {
  apiKey: "...",
  authDomain: "...",
  databaseURL: "...",
  projectId: "...",
  storageBucket: "...",
  messagingSenderId: "...",
  appId: "..."
};

// For Firebase JS SDK v7.20.0 and later, measurementId is optional
// Inicializar Firebase
firebase.initializeApp(firebaseConfig);
var dbRef = firebase.database();
var WeatherApp = document.getElementById('WeatherApp')
var Temperatura=0;
var Presion=0;
var grados =1;

//let dbTemp = firebase.database().ref("Monitor/Temperatura/");
let dbTemp = dbRef.ref("Monitor/Temperatura/");

dbTemp.on('value', function(snapshot) {
  var Temperatura = snapshot.val();
  console.log("la temperatura es", Temperatura);
  Temperatura=snapshot.val();
  if (Temperatura !== null) {
    document.getElementById("TemperaturaId").innerHTML = Temperatura.toFixed(2) + " °C";
  }
});


//let dbPres = firebase.database().ref("Monitor/Presion/");
let dbPres = dbRef.ref("Monitor/Presion/");

dbPres.on('value', function(snapshot) {

  var Presion = snapshot.val();
  console.log("la presion es",Presion);
  Presion=snapshot.val();
  if (Presion !== null) {
    document.getElementById("PresionId").innerHTML = Presion.toFixed(0) + " mmHg";
  }
});

let dbAlt = dbRef.ref("Monitor/Altitude/");

dbAlt.on('value', function(snapshot) {
  var Altitude = snapshot.val();
  console.log("la altitud es", Altitude);
  if (Altitude !== null) {
    document.getElementById("AltitudId").innerHTML = Altitude.toFixed(2) + " m";
  }
});