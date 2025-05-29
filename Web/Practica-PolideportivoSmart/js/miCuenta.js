document.addEventListener('DOMContentLoaded', () => {
  const userEmail = localStorage.getItem('userEmail');
  const userName = localStorage.getItem('userName');
  const fechaRegistro = localStorage.getItem('fechaRegistro');

  if (!userEmail) {
    window.location.href = 'login.html';
    return;
  }

  document.getElementById('userNameDisplay').textContent = `Nombre: ${userName || 'No disponible'}`;
  document.getElementById('userEmailDisplay').textContent = `Correo: ${userEmail}`;
  document.getElementById('fechaRegistroDisplay').textContent = `Fecha de Registro: ${fechaRegistro || 'No disponible'}`;

  document.getElementById('logoutBtn').addEventListener('click', () => {
    localStorage.clear();
    window.location.href = 'login.html';
  });
});
