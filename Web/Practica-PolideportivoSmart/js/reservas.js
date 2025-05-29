document.addEventListener('DOMContentLoaded', () => {
  const BOTON = document.querySelector("#boton");
  const MAIN = document.getElementById('main');

  if (!BOTON) return;

  BOTON.addEventListener('click', async (E) => {
    E.preventDefault();  // evita el envío del formulario

    const NUMEROS = Math.floor(Math.random() * 900) + 100; // 100-999
    const LETRAS = 'abcd';
    const LETRA = LETRAS.charAt(Math.floor(Math.random() * LETRAS.length));

    const CLAVE = `${NUMEROS}${LETRA}`;

    // Recoger valores del formulario
    const ACTIVIDAD = document.getElementById("actividad").value;
    const FECHA = document.getElementById("fecha").value;
    const HORA = document.getElementById("hora").value;
    const USUARIO = document.getElementById("nombre").value;

    const RESERVA = {
      clave: CLAVE,
      actividad: ACTIVIDAD,
      fecha: FECHA,
      hora: HORA,
      usuario: USUARIO
    };

    // Enviar reserva a Orion
    await enviarReserva(RESERVA);

    // Mostrar confirmación al usuario
    MAIN.innerHTML = `
      <h2>Reserva Confirmada</h2>
      <p>Tu contraseña de acceso es:</p>
      <div class="codigo-acceso">${CLAVE}</div>
      <p>Guárdala para poder acceder.</p>
    `;
  });

  async function enviarReserva(RESERVA) {
    const IDENTIDAD = "reserva_" + RESERVA.clave; // id único basado en clave

    const ENTIDAD = {
      id: IDENTIDAD,
      type: "Reserva",
      clave: { type: "Text", value: RESERVA.clave },
      actividad: { type: "Text", value: RESERVA.actividad },
      fecha: { type: "DateTime", value: RESERVA.fecha + "T" + RESERVA.hora + ":00Z" },
      hora: { type: "Text", value: RESERVA.hora },
      usuario: { type: "Text", value: RESERVA.usuario }
    };

    try {
        const RESPONSE = await fetch('http://localhost:3000/proxy/entities', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Fiware-Service': 'smartprovincia',
          'Fiware-ServicePath': '/Practicas2025'
        },
       body: JSON.stringify(ENTIDAD)
        });


      if (RESPONSE.ok) {
        console.log("Reserva enviada correctamente");
      } else {
        const ERROR_TEXT = await RESPONSE.text();
        console.error("Error enviando reserva:", ERROR_TEXT);
      }
    } catch (ERROR) {
      console.error("Error en fetch:", ERROR);
    }
  }

});
