<img width="1892" height="980" alt="image" src="https://github.com/user-attachments/assets/8b84de78-bdd4-4411-a1d6-7e8952b36c85" />
<img width="1914" height="875" alt="image" src="https://github.com/user-attachments/assets/fcfc94d0-6a05-4ced-9fed-2c514653bda9" />

# LOCKSTATE: Quorum Breach

> A browser-based action game where distributed systems theory meets bullet hell. Lock towers, hold consensus, and take down the Warden.

---

## What is this?

LOCKSTATE is a playable prototype built around the concepts behind distributed systems — majority quorum, consensus failure, stale data, and process arbitration — but wrapped in a fast-paced shooting game you can open straight in your browser.

You're defending a node cluster from rogue transactions. Lock enough towers, hold the network's majority vote, and eventually you'll crack the boss's global shield and face the Warden head-on.

---

## Controls

| Input | Action |
|-------|--------|
| `WASD` | Move |
| Mouse | Aim |
| Click | Shoot rogue transactions |
| `E` (near tower) | Lock / unlock tower |
| `Shift` | Dash |
| `H` | Toggle tutorial overlay |
| `P` | Pause / resume |

---

## How to Win

1. **Lock 3 of 5 towers** — majority consensus is your win gate
2. **Hold that majority for 12 seconds** — don't let towers fall offline
3. **Drop the boss's global shield** — only possible once consensus is established
4. **Defeat the Warden** — the final process standing between you and a clean shutdown

---

## The Distributed Systems Bits (In Plain English)

- **Majority locking** — you can't progress without controlling more than half the towers. Just like a real quorum vote.
- **Towers go offline** — sustained transaction attacks can knock a tower out entirely. Watch your consensus count.
- **Shielded boss phase** — the Warden hides behind a global shield that only drops when the network agrees. No consensus, no damage.
- **Confidence decay** — the remote catalog panel gets stale over time. The longer you wait, the less you can trust it.

---

## Project Structure

```
index.html    — App shell and HUD layout
styles.css    — Visual design and responsive UI
game.js       — Full game logic and rendering loop
vercel.json   — Rewrite config for Vercel static deployment
```

---





The `vercel.json` handles routing so all paths resolve to `index.html`.

---

*Built as a prototype. Open in browser and play.*

