const canvas = document.getElementById("game");
const ctx = canvas.getContext("2d");
const startBtn = document.getElementById("startBtn");

const playerHpEl = document.getElementById("playerHp");
const bossHpEl = document.getElementById("bossHp");
const quorumEl = document.getElementById("quorum");
const shieldEl = document.getElementById("shield");
const statusEl = document.getElementById("status");
const catalogEl = document.getElementById("catalog");
const tutorialOverlayEl = document.getElementById("tutorialOverlay");

const world = {
  w: canvas.width,
  h: canvas.height,
};

const keys = new Set();
const mouse = { x: world.w / 2, y: world.h / 2, down: false };

const config = {
  playerSpeed: 235,
  dashSpeed: 600,
  dashDuration: 0.11,
  fireRate: 0.15,
  bulletSpeed: 540,
  enemySpeed: 95,
  enemySpawnEvery: 1.05,
  enemyDamage: 14,
  towerAttackRange: 20,
  quorumSecondsNeeded: 12,
};

let state;

function clamp(v, min, max) {
  return Math.max(min, Math.min(max, v));
}

function dist(a, b) {
  const dx = a.x - b.x;
  const dy = a.y - b.y;
  return Math.hypot(dx, dy);
}

function normalize(x, y) {
  const d = Math.hypot(x, y) || 1;
  return { x: x / d, y: y / d };
}

function makeCatalog() {
  return [
    { name: "Branch-9", confidence: 0.92, age: 0.2 },
    { name: "Branch-12", confidence: 0.66, age: 1.4 },
    { name: "Branch-18", confidence: 0.43, age: 2.6 },
    { name: "Branch-22", confidence: 0.81, age: 0.9 },
  ];
}

function resetGame() {
  state = {
    running: true,
    paused: false,
    showTutorial: true,
    won: false,
    lost: false,
    status: "Mission active. Secure majority consensus.",
    player: {
      x: world.w / 2,
      y: world.h - 80,
      r: 12,
      hp: 100,
      fireCd: 0,
      dashCd: 0,
      dashTime: 0,
      dirX: 0,
      dirY: -1,
    },
    boss: {
      x: world.w / 2,
      y: 100,
      r: 22,
      hp: 300,
      speed: 58,
      pulseCd: 2.5,
    },
    towers: [
      { x: 140, y: 120, hp: 100, locked: false },
      { x: world.w - 140, y: 120, hp: 100, locked: false },
      { x: 160, y: world.h - 130, hp: 100, locked: false },
      { x: world.w - 160, y: world.h - 130, hp: 100, locked: false },
      { x: world.w / 2, y: world.h / 2, hp: 100, locked: false },
    ],
    enemies: [],
    bullets: [],
    lastSpawn: 0,
    quorumSeconds: 0,
    shieldUp: true,
    catalog: makeCatalog(),
    time: 0,
  };
  tutorialOverlayEl.classList.remove("hidden");
  renderCatalog();
}

function spawnEnemy() {
  const side = Math.floor(Math.random() * 4);
  let x = 0;
  let y = 0;
  if (side === 0) {
    x = Math.random() * world.w;
    y = -20;
  } else if (side === 1) {
    x = world.w + 20;
    y = Math.random() * world.h;
  } else if (side === 2) {
    x = Math.random() * world.w;
    y = world.h + 20;
  } else {
    x = -20;
    y = Math.random() * world.h;
  }
  state.enemies.push({ x, y, r: 10, hp: 30, speed: config.enemySpeed + Math.random() * 30 });
}

function lockOrUnlockNearbyTower() {
  const p = state.player;
  let picked = null;
  let best = Infinity;
  for (const t of state.towers) {
    if (t.hp <= 0) continue;
    const d = dist(p, t);
    if (d < 42 && d < best) {
      best = d;
      picked = t;
    }
  }
  if (picked) {
    picked.locked = !picked.locked;
    state.status = picked.locked
      ? "Tower lock vote broadcast."
      : "Tower vote released.";
  }
}

function shoot() {
  const p = state.player;
  if (p.fireCd > 0 || !state.running) return;
  const dir = normalize(mouse.x - p.x, mouse.y - p.y);
  state.bullets.push({
    x: p.x,
    y: p.y,
    vx: dir.x * config.bulletSpeed,
    vy: dir.y * config.bulletSpeed,
    ttl: 1.1,
  });
  p.fireCd = config.fireRate;
}

function updatePlayer(dt) {
  const p = state.player;
  let mx = 0;
  let my = 0;
  if (keys.has("w")) my -= 1;
  if (keys.has("s")) my += 1;
  if (keys.has("a")) mx -= 1;
  if (keys.has("d")) mx += 1;

  if (mx !== 0 || my !== 0) {
    const n = normalize(mx, my);
    p.dirX = n.x;
    p.dirY = n.y;
  }

  let speed = config.playerSpeed;
  if (p.dashTime > 0) {
    p.dashTime -= dt;
    speed = config.dashSpeed;
  }

  p.x += p.dirX * speed * dt * (mx !== 0 || my !== 0 ? 1 : 0);
  p.y += p.dirY * speed * dt * (mx !== 0 || my !== 0 ? 1 : 0);

  p.x = clamp(p.x, p.r, world.w - p.r);
  p.y = clamp(p.y, p.r, world.h - p.r);

  p.fireCd = Math.max(0, p.fireCd - dt);
  p.dashCd = Math.max(0, p.dashCd - dt);

  if (mouse.down) shoot();
}

function updateBoss(dt) {
  const b = state.boss;
  const p = state.player;

  const d = dist(b, p);
  if (d > 120) {
    const n = normalize(p.x - b.x, p.y - b.y);
    b.x += n.x * b.speed * dt;
    b.y += n.y * b.speed * dt;
  }

  b.pulseCd -= dt;
  if (b.pulseCd <= 0) {
    b.pulseCd = 2.5;
    for (let i = 0; i < 14; i++) {
      const a = (Math.PI * 2 * i) / 14;
      state.bullets.push({
        x: b.x,
        y: b.y,
        vx: Math.cos(a) * 180,
        vy: Math.sin(a) * 180,
        ttl: 1.8,
        hostile: true,
      });
    }
  }
}

function updateBullets(dt) {
  const b = state.boss;
  for (const bullet of state.bullets) {
    bullet.x += bullet.vx * dt;
    bullet.y += bullet.vy * dt;
    bullet.ttl -= dt;

    if (bullet.hostile) {
      if (dist(bullet, state.player) < state.player.r + 4) {
        state.player.hp -= 9;
        bullet.ttl = 0;
      }
      continue;
    }

    for (const e of state.enemies) {
      if (e.hp <= 0) continue;
      if (dist(bullet, e) < e.r + 4) {
        e.hp -= 22;
        bullet.ttl = 0;
        break;
      }
    }

    if (bullet.ttl > 0 && !state.shieldUp && dist(bullet, b) < b.r + 4) {
      b.hp -= 7;
      bullet.ttl = 0;
    }
  }

  state.bullets = state.bullets.filter(
    (bullet) => bullet.ttl > 0 && bullet.x >= -40 && bullet.x <= world.w + 40 && bullet.y >= -40 && bullet.y <= world.h + 40
  );
  state.enemies = state.enemies.filter((e) => e.hp > 0);
}

function chooseEnemyTarget(enemy) {
  const p = state.player;
  let tx = p.x;
  let ty = p.y;
  let best = dist(enemy, p);

  for (const t of state.towers) {
    if (t.hp <= 0) continue;
    const d = dist(enemy, t);
    if (d < best) {
      best = d;
      tx = t.x;
      ty = t.y;
    }
  }

  return { x: tx, y: ty };
}

function updateEnemies(dt) {
  for (const e of state.enemies) {
    const target = chooseEnemyTarget(e);
    const n = normalize(target.x - e.x, target.y - e.y);
    e.x += n.x * e.speed * dt;
    e.y += n.y * e.speed * dt;

    if (dist(e, state.player) < e.r + state.player.r) {
      state.player.hp -= config.enemyDamage * dt;
    }

    for (const t of state.towers) {
      if (t.hp <= 0) continue;
      if (dist(e, t) < config.towerAttackRange) {
        t.hp -= 20 * dt;
        if (t.hp <= 0) {
          t.hp = 0;
          t.locked = false;
          state.status = "A tower went offline due to transaction flood.";
        }
      }
    }
  }
}

function updateQuorum(dt) {
  const lockedAlive = state.towers.filter((t) => t.locked && t.hp > 0).length;
  const alive = state.towers.filter((t) => t.hp > 0).length;

  if (lockedAlive >= 3) {
    state.quorumSeconds += dt;
    state.status = "Majority consensus in progress.";
  } else {
    state.quorumSeconds = Math.max(0, state.quorumSeconds - dt * 1.4);
  }

  if (state.quorumSeconds >= config.quorumSecondsNeeded) {
    state.shieldUp = false;
    state.status = "Global shield dropped. Damage the Warden.";
  }

  if (alive < 3 && state.running) {
    state.lost = true;
    state.running = false;
    state.status = "Mission failed: fewer than 3 towers remain online.";
  }
}

function updateCatalog(dt) {
  for (const note of state.catalog) {
    note.age += dt * (0.5 + Math.random() * 0.6);
    note.confidence -= dt * 0.012;
    note.confidence = clamp(note.confidence, 0.08, 0.99);
  }

  if (Math.random() < dt * 0.4) {
    const idx = Math.floor(Math.random() * state.catalog.length);
    state.catalog[idx].age = 0;
    state.catalog[idx].confidence = clamp(state.catalog[idx].confidence + 0.28, 0, 0.99);
  }

  renderCatalog();
}

function renderCatalog() {
  catalogEl.innerHTML = "";
  for (const note of state.catalog) {
    const div = document.createElement("div");
    div.className = "note";

    let tag = "fresh";
    let label = "FRESH";
    if (note.confidence < 0.55) {
      tag = "stale";
      label = "STALE";
    }
    if (note.confidence < 0.3) {
      tag = "risky";
      label = "RISKY";
    }

    div.innerHTML = `
      <div><strong>${note.name}</strong></div>
      <div class="meta">age: ${note.age.toFixed(1)}m | confidence: ${(note.confidence * 100).toFixed(0)}%</div>
      <span class="tag ${tag}">${label}</span>
    `;
    catalogEl.appendChild(div);
  }
}

function drawBackground() {
  ctx.fillStyle = "#071018";
  ctx.fillRect(0, 0, world.w, world.h);

  ctx.strokeStyle = "rgba(90, 150, 175, 0.22)";
  for (let x = 0; x < world.w; x += 48) {
    ctx.beginPath();
    ctx.moveTo(x, 0);
    ctx.lineTo(x, world.h);
    ctx.stroke();
  }
  for (let y = 0; y < world.h; y += 48) {
    ctx.beginPath();
    ctx.moveTo(0, y);
    ctx.lineTo(world.w, y);
    ctx.stroke();
  }
}

function drawTowers() {
  for (const t of state.towers) {
    const online = t.hp > 0;
    ctx.beginPath();
    ctx.arc(t.x, t.y, 18, 0, Math.PI * 2);
    ctx.fillStyle = !online ? "#3b4150" : t.locked ? "#19d4a1" : "#2e7fb3";
    ctx.fill();

    ctx.strokeStyle = "#d5ecff";
    ctx.lineWidth = 2;
    ctx.stroke();

    const hpWidth = 36;
    const ratio = clamp(t.hp / 100, 0, 1);
    ctx.fillStyle = "#24313f";
    ctx.fillRect(t.x - hpWidth / 2, t.y + 22, hpWidth, 5);
    ctx.fillStyle = ratio > 0.5 ? "#4ade80" : ratio > 0.25 ? "#fbbf24" : "#fb7185";
    ctx.fillRect(t.x - hpWidth / 2, t.y + 22, hpWidth * ratio, 5);
  }
}

function drawEntities() {
  const p = state.player;
  const b = state.boss;

  for (const bullet of state.bullets) {
    ctx.beginPath();
    ctx.arc(bullet.x, bullet.y, 3.5, 0, Math.PI * 2);
    ctx.fillStyle = bullet.hostile ? "#ff8ea2" : "#7ce4ff";
    ctx.fill();
  }

  for (const e of state.enemies) {
    ctx.beginPath();
    ctx.arc(e.x, e.y, e.r, 0, Math.PI * 2);
    ctx.fillStyle = "#f28775";
    ctx.fill();
  }

  ctx.beginPath();
  ctx.arc(b.x, b.y, b.r, 0, Math.PI * 2);
  ctx.fillStyle = state.shieldUp ? "#6a8cff" : "#fb7e54";
  ctx.fill();

  if (state.shieldUp) {
    ctx.beginPath();
    ctx.arc(b.x, b.y, b.r + 9, 0, Math.PI * 2);
    ctx.strokeStyle = "rgba(124, 160, 255, 0.8)";
    ctx.lineWidth = 3;
    ctx.stroke();
  }

  ctx.beginPath();
  ctx.arc(p.x, p.y, p.r, 0, Math.PI * 2);
  ctx.fillStyle = "#46d5ff";
  ctx.fill();
}

function drawOverlay() {
  if (state.running && !state.paused) return;
  ctx.fillStyle = "rgba(3, 10, 14, 0.72)";
  ctx.fillRect(0, 0, world.w, world.h);

  if (state.paused && state.running) {
    ctx.textAlign = "center";
    ctx.fillStyle = "#9dd2ff";
    ctx.font = "700 42px Segoe UI";
    ctx.fillText("PAUSED", world.w / 2, world.h / 2 - 10);
    ctx.fillStyle = "#d4e9f8";
    ctx.font = "500 20px Segoe UI";
    ctx.fillText("Press P to resume", world.w / 2, world.h / 2 + 34);
    return;
  }

  ctx.textAlign = "center";
  ctx.fillStyle = state.won ? "#62f7b3" : "#ff9ca8";
  ctx.font = "700 42px Segoe UI";
  ctx.fillText(state.won ? "MISSION SUCCESS" : "MISSION FAILED", world.w / 2, world.h / 2 - 10);

  ctx.fillStyle = "#d4e9f8";
  ctx.font = "500 20px Segoe UI";
  ctx.fillText("Click Start / Restart Mission to play again", world.w / 2, world.h / 2 + 34);
}

function updateHud() {
  const p = state.player;
  const b = state.boss;
  const lockedAlive = state.towers.filter((t) => t.locked && t.hp > 0).length;

  playerHpEl.textContent = `Player HP: ${Math.max(0, p.hp).toFixed(0)}`;
  bossHpEl.textContent = `Warden HP: ${Math.max(0, b.hp).toFixed(0)}`;
  quorumEl.textContent = `Majority Lock: ${lockedAlive}/5 | Consensus Time: ${state.quorumSeconds.toFixed(1)}s / ${config.quorumSecondsNeeded}s`;
  shieldEl.textContent = `Global Shield: ${state.shieldUp ? "ACTIVE" : "DOWN"}`;
  statusEl.textContent = `Status: ${state.status}`;
}

function update(dt) {
  if (!state.running || state.paused) return;

  state.time += dt;
  state.lastSpawn += dt;
  if (state.lastSpawn >= config.enemySpawnEvery) {
    state.lastSpawn = 0;
    spawnEnemy();
  }

  updatePlayer(dt);
  updateBoss(dt);
  updateEnemies(dt);
  updateBullets(dt);
  updateQuorum(dt);
  updateCatalog(dt);

  if (state.player.hp <= 0 && state.running) {
    state.running = false;
    state.lost = true;
    state.status = "Mission failed: player process terminated.";
  }

  if (state.boss.hp <= 0 && state.running) {
    state.running = false;
    state.won = true;
    state.status = "Mission success: Warden process removed.";
  }

  updateHud();
}

function render() {
  drawBackground();
  drawTowers();
  drawEntities();
  drawOverlay();
}

let last = performance.now();
function gameLoop(now) {
  const dt = Math.min((now - last) / 1000, 0.033);
  last = now;
  update(dt);
  render();
  requestAnimationFrame(gameLoop);
}

window.addEventListener("keydown", (e) => {
  const key = e.key.toLowerCase();
  if (["w", "a", "s", "d", "e", "shift", "h", "p"].includes(key)) {
    e.preventDefault();
  }

  keys.add(key);

  if (key === "e" && state.running) {
    lockOrUnlockNearbyTower();
  }

  if (key === "h") {
    state.showTutorial = !state.showTutorial;
    tutorialOverlayEl.classList.toggle("hidden", !state.showTutorial);
    if (state.running) {
      state.status = state.showTutorial ? "Tutorial overlay enabled." : "Tutorial overlay hidden.";
      updateHud();
    }
  }

  if (key === "p" && state.running) {
    state.paused = !state.paused;
    state.status = state.paused ? "Paused." : "Mission resumed.";
    updateHud();
  }

  if (key === "shift" && state.running && !state.paused) {
    const p = state.player;
    if (p.dashCd <= 0) {
      p.dashTime = config.dashDuration;
      p.dashCd = 1.5;
    }
  }
});

window.addEventListener("keyup", (e) => {
  keys.delete(e.key.toLowerCase());
});

canvas.addEventListener("mousemove", (e) => {
  const r = canvas.getBoundingClientRect();
  mouse.x = ((e.clientX - r.left) / r.width) * canvas.width;
  mouse.y = ((e.clientY - r.top) / r.height) * canvas.height;
});

canvas.addEventListener("mousedown", () => {
  mouse.down = true;
});

window.addEventListener("mouseup", () => {
  mouse.down = false;
});

startBtn.addEventListener("click", () => {
  resetGame();
});

resetGame();
updateHud();
requestAnimationFrame(gameLoop);
