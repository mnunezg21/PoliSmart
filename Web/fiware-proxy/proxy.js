import express from 'express';
import fetch from 'node-fetch';
import cors from 'cors';

const app = express();

const PORT = 3000;

const CB_URL = 'https://cb.iotplatform.telefonica.com:10027/v2';
const FIWARE_SERVICE = 'smartprovincia';
const FIWARE_SERVICEPATH = '/Practicas2025';

const X_AUTH_TOKEN = 'gAAAAABoNrZv56uoiArkPN-LfFJwW1D54KG8M37eqcigwC_BZsexuP8OxUVIkoVZIQP6IzzW80GatQpDavVr3BRALTGaTHho3JXhPr3_iaOia-KUcnJ_guLoEz4mLLn417r3YrwmbIGZNLLkkrJ4wH8YiJM-OeDSNhMEyWEfpW5lc1nENMFoLfA';

app.use(cors());
app.use(express.json());

app.use('/proxy', async (req, res) => {
  try {
    const path = req.originalUrl.replace(/^\/proxy/, '');
    const url = CB_URL + path;
    const method = req.method;

    // Solo envía body si no es GET, DELETE ni HEAD
    const bodyData = ['GET', 'DELETE', 'HEAD'].includes(method) ? undefined : JSON.stringify(req.body);

    // Arma headers, sin Content-Type para GET, DELETE y HEAD
    const headersToSend = {
      'Fiware-Service': FIWARE_SERVICE,
      'Fiware-ServicePath': FIWARE_SERVICEPATH,
      'X-Auth-Token': X_AUTH_TOKEN,
    };

    if (!['GET', 'DELETE', 'HEAD'].includes(method)) {
      headersToSend['Content-Type'] = 'application/json';
    }

    console.log(`Proxy >> ${method} ${url}`);
    if (bodyData) console.log(`Body enviado: ${bodyData}`);
    console.log('Headers enviados:', headersToSend);

    const response = await fetch(url, {
      method,
      headers: headersToSend,
      body: bodyData,
    });

    const responseText = await response.text();

    console.log(`Orion respondió con status: ${response.status}`);
    console.log(`Respuesta de Orion: ${responseText}`);

    res.status(response.status);
    res.set('Content-Type', response.headers.get('content-type') || 'text/plain');
    res.send(responseText);

  } catch (error) {
    console.error('Error en proxy:', error);
    res.status(500).send('Error interno en proxy');
  }
});

app.listen(PORT, () => {
  console.log(`Proxy corriendo en http://localhost:${PORT}`);
});
