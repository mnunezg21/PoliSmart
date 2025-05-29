document.addEventListener('DOMContentLoaded', () => {
  const form = document.querySelector('form');
  const main = document.querySelector('main');

  function emailToId(email) {
    return email.toLowerCase()
      .replace(/@/g, '-at-')
      .replace(/\./g, '-dot-');
  }

  form.addEventListener('submit', async (e) => {
    e.preventDefault();

    const email = document.getElementById('email').value.trim();
    const password = document.getElementById('password').value;
    const nombre = document.getElementById('nombre').value.trim();

    if (!email || !password || !nombre) {
      alert('Por favor completa todos los campos.');
      return;
    }

    const entityId = `User:${emailToId(email)}`;

    const entidad = {
      id: entityId,
      type: "User",
      email: { type: "Text", value: email },
      password: { type: "Text", value: password },
      nombre: { type: "Text", value: nombre }
    };

    try {
      const response = await fetch('http://localhost:3000/proxy/entities', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Fiware-Service': 'smartprovincia',
          'Fiware-ServicePath': '/Practicas2025'
        },
        body: JSON.stringify(entidad)
      });

      if (response.ok) {
        // Guardar datos localmente
        localStorage.setItem('userName', nombre);
        localStorage.setItem('userEmail', email);
        const fechaActual = new Date().toLocaleDateString();
        localStorage.setItem('fechaRegistro', fechaActual);

        main.innerHTML = `
          <h2>Registro exitoso</h2>
          <p>Tu cuenta ha sido creada correctamente.</p>
          <a href="login.html">Ir a Iniciar Sesión</a>
        `;
      } else if (response.status === 409) {
        alert('El usuario ya existe.');
      } else {
        alert('Error al registrar el usuario.');
      }
    } catch (error) {
      console.error('Error al registrar usuario:', error);
      alert('Error de conexión con el servidor.');
    }
  });
});
