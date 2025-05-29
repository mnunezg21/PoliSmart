document.addEventListener('DOMContentLoaded', () => {
  const accesoDiv = document.getElementById('acceso-usuario');
  if (!accesoDiv) return; // Por si alguna página no tiene el div

  // Insertamos botones base
  accesoDiv.innerHTML = `
    <button id="loginBtn" class="boton-usuario">Iniciar Sesión</button>
    <button id="registerBtn" class="boton-usuario">Registrarse</button>
    <button id="accountBtn" class="boton-usuario" style="display:none;">Mi Cuenta</button>
    <button id="logoutBtn" class="boton-usuario" style="display:none;">Cerrar Sesión</button>
  `;

  const userEmail = localStorage.getItem('userEmail');

  if (userEmail) {
    // Usuario logueado: ocultar login/registro, mostrar cuenta y logout
    document.getElementById('loginBtn').style.display = 'none';
    document.getElementById('registerBtn').style.display = 'none';
    document.getElementById('accountBtn').style.display = 'inline-block';
    document.getElementById('logoutBtn').style.display = 'inline-block';

    document.getElementById('accountBtn').addEventListener('click', () => {
      window.location.href = 'miCuenta.html';
    });

    document.getElementById('logoutBtn').addEventListener('click', () => {
      localStorage.removeItem('userEmail');
      window.location.href = 'login.html';
    });
  } else {
    // No logueado: mostrar login/registro, ocultar cuenta/logout
    document.getElementById('loginBtn').style.display = 'inline-block';
    document.getElementById('registerBtn').style.display = 'inline-block';
    document.getElementById('accountBtn').style.display = 'none';
    document.getElementById('logoutBtn').style.display = 'none';

    document.getElementById('loginBtn').addEventListener('click', () => {
      window.location.href = 'login.html';
    });

    document.getElementById('registerBtn').addEventListener('click', () => {
      window.location.href = 'registro.html';
    });
  }
});
