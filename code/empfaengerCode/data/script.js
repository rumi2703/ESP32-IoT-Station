const ctx = document.getElementById('chart');

const chart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: [],
    datasets: [{
      label: 'Temperatur',
      data: []
    }]
  }
});

async function update() {
  const res = await fetch('/data');
  const json = await res.json();

  chart.data.labels.push("");
  chart.data.datasets[0].data.push(json.temp);

  chart.update();
}

setInterval(update, 2000);