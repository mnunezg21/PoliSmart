document.addEventListener('DOMContentLoaded', () => {
  const form = document.querySelector('form');
  const main = document.querySelector('main');

  function emailToId(email) {
    return email.toLowerCase()
      .replace(/@/g, '-at-')
      .replace(/\./g, '-dot-')
      .replace(/:/g, '_'); // Reemplazar ':' por '_'
  }

  form.addEventListener('submit', async (e) => {
    e.preventDefault();

    const email = document.getElementById('email').value.trim();
    const password = document.getElementById('password').value;

    if (!email || !password) {
      alert('Por favor completa todos los campos.');
      return;
    }

    const rawId = `User:${emailToId(email)}`;
    const encodedId = encodeURIComponent(rawId);

    try {
      const response = await fetch(`http://localhost:3000/proxy/entities/${encodedId}`, {
        method: 'GET'
        // ❌ No agregamos headers aquí, el proxy los agrega
      });

      if (!response.ok) {
        if (response.status === 404) {
          alert('Usuario no encontrado.');
        } else {
          alert('Error al consultar el servidor.');
        }
        return;
      }

      const resultado = await response.json();

      if (resultado.password && resultado.password.value === password) {
        // Guardar sesión en localStorage
        localStorage.setItem('userEmail', email);

        // Redirigir al index.html
        window.location.href = 'index.html';

      } else {
        alert('Contraseña incorrecta.');
      }
    } catch (error) {
      console.error('Error al consultar Orion:', error);
      alert('Error de conexión al servidor.');
    }
  });
});
