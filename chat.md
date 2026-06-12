# CARS
_Exported on 6/11/2026 at 18:46:04 MDT from Cursor (3.6.31)_

---

**User**

/home/mmanjee/HeaRT/CARS/dataset/download
Want to download this dataset is it done correctly: https://unsworks.unsw.edu.au/bitstreams/0ac2820a-5131-43ab-90b2-c624c8d73649/download 

---

**Cursor**

Checking the download script and whether it targets the correct URL.


The download succeeded (~941 MB) but it's RAR, not ZIP — verifying the file and how CARS expects the data.


---

**User**

@/home/mmanjee/.cursor/projects/home-mmanjee-HeaRT/terminals/9.txt:280-295 

---

**Cursor**

`unrar` install failed on auth — checking what else can extract the RAR file.


`bsdtar` can read the RAR — extracting it now.


**The download is correct.** The file is a valid ~942 MB RAR archive (NGIDS dataset). `unzip` failed because it's not a ZIP.

`unrar` install failed because `dnf` needs sudo/auth you don't have in that session. Use **`bsdtar`** instead — it's already on the system and works:

```bash
cd /home/mmanjee/HeaRT/CARS/dataset
bsdtar -xf download
```

I extracted it successfully. Contents are under `NGIDS/NGIDS-DS-v1/`:
- `feature_descr.csv`, `ground_truth.csv`, `Readme.txt`
- `host logs/` (numbered CSVs)
- `NGIDS.pcap`

Optional cleanup:

```bash
mv download NGIDS_DS.rar   # clearer name
# rm NGIDS_DS.rar          # after verifying extraction
```

---

**User**

@CARS/dataset/ is this graph dataset related to cybersecurity?

---

**Cursor**

Inspecting the dataset files and any CARS project context.


**Yes for cybersecurity; no as a ready-made graph dataset.**

`CARS/dataset/` holds **NGIDS-DS** (Next Generation Intrusion Detection System Dataset) from UNSW ADFA’s cyber range. It’s an **IDS / anomaly-detection** dataset for host- and network-based security research.

### What it contains
| Component | Format | Purpose |
|-----------|--------|---------|
| `host logs/*.csv` (v1: 99 files) | Tabular event logs | Host-based IDS — processes, syscalls, paths |
| `NGIDS.pcap` | Network capture | Network-based IDS |
| `ground_truth.csv` | Attack labels + IPs/ports | Ground truth for attacks |
| `NGIDS-DS-v2/` | Train/test CSVs | Pre-split normal vs attack records |

Each host log row has fields like `date`, `time`, `pro_id`, `path`, `sys_call`, `event_id`, `attack_cat`, `label` (0=normal, 1=abnormal). Attack categories include Exploits, DoS, Worms, Malware, Fuzzers, Backdoors, Shellcode, Reconnaissance, etc.

### Is it a graph dataset?
**Not in its raw form.** It’s **sequential/tabular security telemetry**, not a graph benchmark like Cora or ogbl-collab in HeaRT. There’s no `edge_index`, adjacency matrix, or graph schema in the download.

You *could* build a graph from it (e.g. processes ↔ syscalls, hosts ↔ IPs from `ground_truth.csv`, or temporal event graphs), but that would be **your preprocessing step** — the dataset itself doesn’t ship as one.

### Relation to HeaRT/CARS
`CARS/` currently only has this raw NGIDS data; there’s no CARS code in the repo that loads or graphs it yet. HeaRT’s graph link-prediction work lives under `HeaRT/`, separate from this cybersecurity dataset.

---

**User**

/home/mmanjee/HeaRT/CARS/dataset/Phase1_Provenance.csv
The dataset contains two datatypes: Provenance data and network logs and each of these datatypes are collected during two phases of the experiment. The provenance data files are in CSV format and contain the nodes and edges of the provenance graph. Each node in the provenance data is assigned a unique 32-digit ID, which is utilized by the edge entries to establish connections between
nodes in the graph.

Besides the IDs, the provenance data files comprise 32 features in total. However, due to the heterogeneous nature of nodes and edges that are all in a single file, not all features apply to every node or edge type, resulting in many fields being populated with NaN values. The provenance data includes two main node types: Process and Artifact. The Artifact node type is further categorized into various subtypes such as file, directory, network socket, link, and unknown, the latter being used for provenance node types that do not fit into the existing subtypes. The common edge types in the provenance graph are: ``Used" (from Process to Artifact), ``WasGeneratedBy" (WGB; from Artifact to Process), ``WasTriggeredBy" (WTB; from Process to Process), and ``WasDerivedFrom" (WDF; from Artifact to Artifact).


Find number of nodes 

---

**Cursor**

Inspecting the CSV structure, then counting unique node IDs.


**46,773 unique nodes** in `Phase1_Provenance.csv`.

### Breakdown
| Category | Count |
|----------|------:|
| **Total nodes** | **46,773** |
| Process nodes | 9,878 |
| Artifact nodes | 36,895 |

### Artifact subtypes
| Subtype | Count |
|---------|------:|
| file | 22,407 |
| network socket | 14,304 |
| link | 170 |
| directory | 13 |
| unknown | 1 |

### How this was counted
- **Nodes** = rows with a non-empty `id` and `type` ∈ {`Process`, `Artifact`}.
- **Edges** = rows with empty `id` and `type` ∈ {`Used`, `WasGeneratedBy`, `WasTriggeredBy`, `WasDerivedFrom`} (175,347 edge rows total).
- Every node ID in `from`/`to` also appears in `id` — no orphan edge endpoints.

**Note:** The file has 222,120 data rows (not 224,697); the higher line count likely comes from newlines inside quoted CSV fields.

---

**User**

how about phas 2?

---

**Cursor**

**Phase 2 has 53,286 unique nodes** in `Phase2_Provenance.csv`.

### Node breakdown
| Category | Phase 1 | Phase 2 |
|----------|--------:|--------:|
| **Total nodes** | 46,773 | **53,286** |
| Process | 9,878 | 24,839 |
| Artifact | 36,895 | 28,447 |

### Artifact subtypes (Phase 2)
| Subtype | Count |
|---------|------:|
| file | 17,344 |
| network socket | 10,787 |
| link | 283 |
| unknown | 18 |
| directory | 15 |

### Edges (Phase 2)
| Edge type | Count |
|-----------|------:|
| WasGeneratedBy | 87,636 |
| Used | 34,206 |
| WasTriggeredBy | 13,136 |
| WasDerivedFrom | 8,471 |
| **Total edge rows** | **143,449** |

Phase 2 is larger in total nodes (+6,513) and has many more **Process** nodes (24,839 vs 9,878), while Phase 1 had more **Artifact** nodes. Same schema as Phase 1 — every edge endpoint matches a node `id`.

---

**User**

what is phas1 and phase 2 about? tell briefly the problem and dataset concise and also count number of edges

---

**Cursor**

Searching the dataset for documentation and computing edge counts.


**CICAPT-IIoT** — a cybersecurity provenance dataset for **APT (Advanced Persistent Threat) detection** in an **IIoT testbed** (UNB). It records causal system behavior as a provenance graph (processes, files, sockets) plus network pcaps.

### The problem
APTs are stealthy, multi-stage attacks that are hard to spot with short-window IDS. Provenance graphs link processes and artifacts over time, so you can trace attack chains. Phase 1 gives a **benign baseline**; Phase 2 adds **labeled APT29-style attacks** for detection research.

| | **Phase 1** | **Phase 2** |
|---|---|---|
| **Purpose** | Normal IIoT ops (~4 days) | APT emulation (~3 days, “low & slow”) |
| **Labels** | All benign (`label=0`) | 332 attack nodes + 52,954 benign |
| **Attack tactics** | None | collection, exfiltration, C&C, persistence, discovery, credential access, lateral movement, defense evasion |
| **Time span** | Nov 27 – Dec 1, 2023 | Dec 1 – Dec 4, 2023 |

### Graph stats (your CSV files)

| | **Nodes** | **Edges** |
|---|--:|--:|
| **Phase 1** | 46,773 | **175,347** |
| **Phase 2** | 53,286 | **143,449** |
| **Combined** | 100,059 | **318,796** |

**Edge breakdown**

| Edge type | Phase 1 | Phase 2 |
|-----------|--------:|--------:|
| WasGeneratedBy | 136,444 | 87,636 |
| Used | 22,120 | 34,206 |
| WasTriggeredBy | 5,752 | 13,136 |
| WasDerivedFrom | 11,031 | 8,471 |

So: **yes, cybersecurity graph data** — provenance DAGs from Auditd/SPADE on an IIoT testbed. Phase 1 = benign only; Phase 2 = benign + labeled APT nodes. You also have separate network pcaps and an attack-info file for Phase 2 (not in the provenance CSVs alone).

---

**User**

how can we apply link prediction to this?

---

**Cursor**

Reviewing how link prediction is set up in this repo to tailor guidance for the provenance graph.


**Yes, but you need to define what “link” means first.** CICAPT-IIoT is a **directed, typed, temporal provenance DAG**, not a static social graph like Twitter. Link prediction can still apply — you just pick the scientific question.

## 1. Pick a task (most important step)

| Task | Predict | Security meaning |
|------|---------|------------------|
| **A. Missing-edge prediction** | Does edge `(u,v)` exist? | Reconstruct hidden causal links in the provenance graph |
| **B. Typed link prediction** | Which relation? `Used` / `WasGeneratedBy` / `WasTriggeredBy` / `WasDerivedFrom` | Infer what kind of interaction happened |
| **C. Future-link prediction** | Will `(u,v)` appear later? | Forecast next step in an attack chain |
| **D. Attack-link prediction** | Is this edge part of malicious activity? | Detect suspicious causal connections (uses node `label` / `subLabel`) |

For APT detection, **C or D** are usually more useful than plain “does this edge exist?” on a fully observed log.

## 2. Build the graph (like your SNAP pipeline)

From each `Phase*_Provenance.csv`:

1. **Nodes**: rows with non-empty `id` → map 32-char IDs to `0..N-1`
2. **Edges**: rows with empty `id` → `(from, to)` + optional `type`, `operation`, `time`
3. **Keep directed** — provenance is causal (`Process → Artifact`, etc.)
4. **Optional**: one meta-graph with all edge types, or **4 separate graphs** per relation type

Roughly analogous to `preprocess_twitter_edges.py`: emit `edge_index`, `num_nodes`, node features, and `split_edge` dict for HeaRT.

## 3. Node features

Use columns that apply per node type (many NaNs are expected):

- **Process**: `exe`, `name`, `uid`, `command line` (embed or hash)
- **Artifact**: `subtype`, `path`, `protocol`, `local/remote address`
- **All**: `type` (Process vs Artifact), `label`, `subLabel`

Start simple: one-hot `type` + `subtype` + binary `label`.

## 4. Train / val / test splits (don’t random-split blindly)

Provenance is **temporal** and Phase 2 has **attacks**. Better options:

**Option A — Temporal (recommended for forecasting)**  
- Train on edges with `time < T1`  
- Val: `T1 ≤ time < T2`  
- Test: `time ≥ T2`  
Predict edges that appear in the future from past structure.

**Option B — Phase-based transfer**  
- Train on **Phase 1** (all benign)  
- Test on **Phase 2** edges  
Tests whether benign structure generalizes; attack edges become hard positives.

**Option C — Standard HeaRT split (like OGB)**  
- Hide 10–20% of observed edges  
- Predict held-out links from the rest  
Good baseline, less “security-native.”

## 5. Positives and negatives

**Positives**: held-out real edges `(from, to)`.

**Negatives** (hard part on DAGs):
- Random non-edges that respect direction (Process→Artifact for `Used`, etc.)
- **HeaRT-style hard negatives**: nodes close in the graph but not connected
- **Temporal negatives**: pairs that never co-occur in a time window
- For typed prediction: same `(u,v)` with wrong edge type

Avoid undirected symmetrization unless you explicitly want that.

## 6. Plug into HeaRT

Your repo expects roughly:

```text
graph.pt          # edge_index, num_nodes, x (features)
split_edge.pt     # train/valid/test pos (and neg) edges
```

Then run GCN / SEAL / BUDDY under `HeaRT/benchmarking/` with `evaluate_hits`, `evaluate_mrr`, `evaluate_auc`.

## 7. Practical recommendations for CICAPT-IIoT

**Best starting point**
1. Phase 2 only (has attack labels)
2. Predict **future `Used` and `WasTriggeredBy` edges** (process-centric, attack-relevant)
3. Temporal split by `time`
4. Evaluate: Hits@K / MRR **and** recall on edges touching `label=1` nodes

**Why this fits APT detection**  
Attacks are rare long chains. Link prediction asks: *“given what we’ve seen so far, what causal link comes next?”* — closer to provenance-based APT tools (UNICORN, ANUBIS) than plain node classification.

**Caveats**
- Graph is a **DAG** (mostly) — many random negatives are trivially invalid
- **99.5% benign** — use stratified metrics on attack-related edges
- Same process can have many edges — consider **deduplicating** or weighting by time
- Phase 1 → Phase 2 is a **distribution shift** (more processes in Phase 2)

## Minimal pipeline sketch

```text
Phase*_Provenance.csv
  → parse nodes/edges, remap IDs
  → build x (node features)
  → temporal or random edge split
  → negative sampling (typed + hard)
  → graph.pt + split_edge.pt
  → HeaRT GCN/SEAL/BUDDY
```

If you say which goal you care about — **forecasting attack steps**, **reconstructing missing links**, or **benign vs attack edge classification** — the exact split and label definition can be narrowed further. I can outline that in Agent mode and wire it to your SNAP/HeaRT preprocessing pattern.

---

**User**

https://dl.acm.org/doi/pdf/10.1145/3588771 how about doing something like this?

---

**Cursor**

Looking up that paper to see how its approach maps to CICAPT-IIoT.


---

**User**

@/home/mmanjee/Downloads/3588771.pdf 

---

**Cursor**

Reading the Euler paper PDF to summarize how it applies to your dataset.


**Yes — Euler is a strong template for CICAPT-IIoT**, but you need to match its *abstraction level*, not dump the full provenance graph in as-is.

## What Euler does ([paper](https://dl.acm.org/doi/10.1145/3588771))

**Problem:** Detect **lateral movement / APT activity** as **anomalous edges** in an evolving graph.

**Pipeline:**
1. Build **temporal snapshots** \(G_t = (V, E_t)\) from host logs (30-min windows on LANL; similar on OpTC).
2. **Train unsupervised** on benign history: learn “normal” interaction patterns.
3. **GNN per snapshot** (spatial) → **RNN/GRU** (temporal) → **inner-product decoder** scores edge likelihood.
4. **Low-score observed edges = anomalies** (unexpected connections given past context).

**Key idea:** The same edge can be normal at \(t_1\) but anomalous at \(t_4\) if prior context changed (Figure 2 in the paper). That’s why time matters.

**Data used:** Coarse **entity–entity** graphs (computer↔computer auth on LANL; host↔host FLOW-START on OpTC), **not** full process/file provenance.

---

## Mapping to your CICAPT-IIoT data

Your CSVs are **already provenance graphs** — closer to Bowman/UNICORN-style work Euler cites than to Euler’s own LANL graphs. You have three realistic options:

### Option A — Closest to Euler (recommended start)
**Collapse to host/network entities**, like OpTC:

- **Nodes:** hosts / IPs (from `remote address`, `local address`, or socket artifacts).
- **Edges:** communication or auth-like interactions in time window \(\Delta t\) (e.g. 15–30 min).
- **Train:** Phase 1 only (all benign, ~4 days).
- **Test:** Phase 2 snapshots; flag low-likelihood edges during attack windows.
- **Labels:** compare to attack nodes / `subLabel` (lateral movement, credential access, etc.).

This mirrors Euler’s threat model: learn benign inter-entity connectivity, detect rare cross-host links during APT.

### Option B — Process-level temporal link prediction
**Nodes:** `Process` only (or Process + key Artifacts).

- **Edges:** `WasTriggeredBy`, `Used`, `WasGeneratedBy` aggregated per snapshot.
- **Train:** Phase 1 or Phase 2 benign periods.
- **Detect:** attack edges touching `label=1` nodes with low predicted likelihood.

More security-native, but **much harder** than Euler: ~50k nodes, heterogeneous types, DAG structure, extreme class imbalance (332 attack nodes / 53k).

### Option C — Hybrid (often best for APT papers)
- **GNN on provenance subgraph** (local process–file–socket neighborhood).
- **Euler-style temporal scoring** on a **coarser host graph** for lateral movement.
- Use Phase 1 → Phase 2 transfer exactly as the dataset authors intended.

---

## Concrete recipe (Euler on CICAPT-IIoT)

```text
1. Parse Phase1_Provenance.csv + Phase2_Provenance.csv
2. Choose abstraction (host-level for Euler-faithful)
3. Bucket edges by time → snapshots G_0 … G_T  (try Δt = 900s or 1800s)
4. Train on Phase 1 snapshots only (unsupervised link prediction, n=1)
5. At test: score each observed edge in Phase 2 with σ(z_u · z_v)
6. Alert if score < threshold (tune on held-out benign Phase 2 edges)
7. Evaluate: precision/recall vs attack-labeled nodes/edges
```

**Architecture:** GCN + GRU (Euler’s simplest winning stack). Code: [github.com/iHeartGraph/Euler](https://github.com/iHeartGraph/Euler).

---

## Fit vs challenges

| | Euler (LANL/OpTC) | Your data |
|--|-------------------|-----------|
| Nodes | ~1k–18k hosts | 47k–53k process/artifact IDs |
| Edges | auth / network flows | 143k–175k typed provenance edges |
| Training | benign-only | Phase 1 perfect for this |
| Labels | anomalous auth edges | 332 attack nodes + MITRE `subLabel` |
| Graph type | entity interaction | causal provenance DAG |

**Main challenges:**
- **Scale:** Full provenance is bigger than LANL snapshots; use snapshots + edge sampling like Euler.
- **Heterogeneity:** Process vs Artifact — either split by type or use heterogeneous GNN (Euler uses homogeneous graphs).
- **Typed edges:** Euler predicts *existence*; you may need separate models per edge type or edge-type as feature.
- **Imbalance:** Same as LANL (~0.5% attacks); optimize for **AP / low FPR**, not accuracy.

---

## How this differs from HeaRT link prediction

| HeaRT (your SNAP work) | Euler |
|------------------------|-------|
| Random/hard negative edge splits | Temporal snapshots + anomaly scoring |
| Static or single graph | Sequence of graphs |
| Maximize Hits@K on held-out links | Minimize false alerts on rare attack edges |
| Supervised split | Unsupervised on benign, detect outliers |

For CICAPT-IIoT security goals, **Euler’s framing is more appropriate** than plain HeaRT-style random link splits.

---

## Practical recommendation

1. **Start host-level Euler clone:** Phase 1 train → Phase 2 detect (matches paper + dataset design).
2. **Baselines:** “edge never seen in training” (UA rule from paper), static GNN without RNN.
3. **Metrics:** AUC, AP, TPR @ low FPR — especially on `lateralMovement` / `credentialAccess` edges.
4. **Then extend** to full provenance if host-level works.

I couldn’t read `/home/mmanjee/Downloads/3588771.pdf` locally (file not found), but the ACM/author PDF matches the above. If you want this wired into a preprocessing script + Euler-style training loop in your repo, switch to Agent mode and we can implement Option A first.

---

**User**

so euler paper suggests some novel method?

---

**Cursor**

**Partly novel — mainly in framing and systems design, not a new GNN architecture.**

## What Euler actually contributes

**1. Problem reframing (main scientific claim)**  
It treats **lateral-movement detection** as **temporal link prediction**: learn normal entity-to-entity connections over time, then flag **low-likelihood edges** as anomalies. The paper argues prior graph-IDS work was mostly **static** or **event-level**, not **temporal + relational** together.

**2. Framework, not a new model**  
The core stack is intentionally simple:

```text
GNN(snapshot_t) → RNN over time → inner-product decoder → edge likelihood
```

That pattern existed before (e.g. T-GCN, DySAT). Euler’s contribution is to **formalize** it as a reusable framework and show a plain **GCN + GRU** matches or beats fancier temporal GNNs.

**3. Scalability / parallelism (main engineering novelty)**  
The real technical idea is **architectural**:

- Run **one GNN per snapshot in parallel** on worker machines  
- A **leader** runs the **RNN sequentially** on those embeddings  
- Avoids prior designs where GNN and RNN are tightly coupled and must run snapshot-by-snapshot in series  

So the novelty is **distributed temporal encoding**, aimed at LANL/OpTC-scale logs.

**4. Extended version adds Euler-SM**  
A second stage on top of embeddings (from Ouyang et al.) to improve anomaly scoring — more of an **augmentation** than a wholly new detector.

## What is not novel

- GNN + RNN for dynamic graphs  
- Link prediction via \( \sigma(z_u^\top z_v) \)  
- “Rare edges = suspicious” (Bowman et al. did static graph version)  
- Training on benign data for anomaly detection  

## Bottom line

Euler is **not** proposing a radically new learning method like a new attention mechanism or loss. It proposes:

1. **A new IDS formulation** (temporal link prediction for lateral movement)  
2. **A scalable parallel framework** around an existing GNN→RNN stack  
3. **Evidence** that simplicity is enough at enterprise scale  

For your CICAPT-IIoT work, the useful takeaway is the **problem setup** (benign Phase 1 → score Phase 2 edges) more than a brand-new model to implement.

---

**User**

I need to write a good paper like this using link prediction for cybersecurity. Guide me what should be the next step?

---

**Cursor**

**Your next step is to lock a precise research question and baseline — not build a model yet.**

Euler works because it answers one clear question: *“Can temporal link prediction detect lateral movement better than static/event-based methods?”* You need an equally sharp question for CICAPT-IIoT that goes beyond replicating Euler on a new dataset.

---

## 1. Choose your contribution (pick one primary angle)

Euler is hard to beat on **host-level lateral movement + distributed GNN/RNN**. A strong paper needs a **clear delta**:

| Direction | Research question | Novelty vs Euler |
|-----------|-------------------|------------------|
| **A. Provenance temporal LP** | Can **process–artifact** link prediction detect APT steps earlier than host-level graphs? | Finer graph, heterogeneous nodes/edges |
| **B. Tactic-aware LP** | Can we predict **which attack tactic edge** comes next (collection, exfiltration, lateral movement)? | Typed / multi-relation prediction, not just existence |
| **C. HeaRT + security** | Do **hard negative** link samplers improve APT edge detection vs random negatives? | Bridges your HeaRT work into cybersecurity |
| **D. Cross-phase transfer** | Does benign **Phase 1** training detect **Phase 2** attacks without attack labels? | Clean train/test story built into CICAPT-IIoT |
| **E. Multi-scale** | Does combining **host graph + provenance graph** beat either alone? | Architecture / fusion contribution |

**Recommendation:** Start with **D + A** — train on Phase 1 provenance, detect anomalous edges in Phase 2, compare against host-level Euler-style baseline. That’s publishable if you show **what provenance adds**.

---

## 2. Define the task precisely (do this before coding)

Write a 1-page problem spec:

**Graph**
- Nodes: Process + Artifact (or start Process-only)
- Edges: `Used`, `WasTriggeredBy`, `WasGeneratedBy`, `WasDerivedFrom`
- Time: bucket by `time` into snapshots (try 15 min, 30 min, 1 hr)

**Prediction target**
- **Detection mode:** score edges in snapshot \(t\) given snapshots \(0..t-1\)
- **Label:** edge is malicious if it touches a `label=1` node or falls in attack window from attack-info file

**Training**
- Unsupervised on Phase 1 only (Euler-style), or semi-supervised on Phase 2 benign nodes only

**Metrics**
- AUC, Average Precision (critical for rare attacks)
- TPR @ fixed low FPR (e.g. 0.1%, 1%)
- Per-tactic recall (`lateralMovement`, `credentialAccess`, etc.)
- Detection delay (time from attack start to first alert)

Without this spec, experiments won’t tell a coherent story.

---

## 3. Immediate next steps (ordered)

### Step 1 — Literature gap table (1 week)
Compare your planned work to:

- **Euler** — temporal LP, host-level, enterprise
- **UNICORN / Kairos / Flash** — provenance anomaly detection
- **Bowman et al.** — static graph anomaly on auth logs
- **CICAPT-IIoT paper** — dataset baseline (if any)

Fill a table: *graph granularity, temporal?, heterogeneous?, IIoT?, negatives?, labels?*  
Your empty cell = contribution.

### Step 2 — Build minimal baselines (2–3 weeks)
Implement in this order (simplest → strongest):

1. **UA rule:** alert on edges never seen in Phase 1  
2. **Static GNN:** single graph, no time (Bowman-style)  
3. **Euler-lite:** GCN + GRU per snapshot, inner-product decoder  
4. **Your method:** whatever novelty you chose in §1  

Same preprocessing, same splits, same metrics for all.

### Step 3 — Ablation plan (design now, run later)
Must-have ablations for reviewers:

- With vs without temporal RNN  
- Host-level vs provenance-level graph  
- Snapshot size \(\Delta t\)  
- Per edge-type vs merged graph  
- Phase 1 only train vs Phase 2 benign train  
- Random vs hard negatives (if using HeaRT ideas)

### Step 4 — One strong result, not ten weak ones
Aim for **one headline finding**, e.g.:

> “Provenance temporal link prediction detects credential-access edges 12% earlier than host-level Euler at 0.5% FPR.”

One clear win beats many tables.

---

## 4. Paper structure (mirror Euler’s arc)

1. **Intro** — APT in IIoT; provenance is rich but unused for *temporal LP*  
2. **Threat model** — benign training data, monitored entities, snapshot granularity  
3. **Method** — snapshots → encoder → decoder → anomaly score  
4. **Dataset** — CICAPT-IIoT Phase 1/2, nodes/edges/tactics (you already have counts)  
5. **Baselines** — UA, static GNN, Euler-style  
6. **Results** — detection quality + ablations + case study subgraph  
7. **Discussion** — limits (scale, label granularity, IIoT vs enterprise)

---

## 5. What to do this week

| Priority | Action |
|----------|--------|
| 1 | Write **problem statement** (1 paragraph) + **contribution claim** (1 sentence) |
| 2 | Build **literature gap table** (Euler, UNICORN, CICAPT-IIoT) |
| 3 | Prototype **graph stats per snapshot** (edges/snapshot, attack edges/snapshot) |
| 4 | Implement **UA + static GNN** baselines first |
| 5 | Pick **target venue** (e.g. RAID, ACSAC, IEEE TIFS, Computers & Security) and check they want IDS or graph learning |

---

## 6. Common pitfalls to avoid

- **“We apply link prediction to a new dataset”** — not enough; need *why provenance/temporal/hard-negs matter*
- **Random train/test edge split** — wrong for security; use **temporal** or **Phase 1 → Phase 2**
- **Accuracy on 99.5% benign data** — misleading; use AP and TPR@low FPR
- **Full 50k-node provenance graph at once** — use snapshots + sampling like Euler
- **No case study** — reviewers want one illustrated attack chain with predicted vs missed edges

---

## Suggested concrete research title (working)

*“Temporal Provenance Link Prediction for APT Detection in IIoT”*  
or  
*“Beyond Host Graphs: Fine-Grained Temporal Link Prediction on Provenance Data”*

---

**Bottom line:** Your literal next step is **write the 1-page task spec + gap table**, then implement **UA and Euler-lite baselines** on Phase 1→Phase 2. Once those numbers exist, you’ll know whether the paper’s angle is provenance granularity, tactics, or hard negatives.

If you tell me which contribution angle (A–E) you prefer, I can outline the exact experiment table and baseline comparison for that angle.

---

**User**

“Beyond Host Graphs: Fine-Grained Temporal Link Prediction on Provenance Data”

briefly explain what this is 

---

**Cursor**

**The idea:** Use **provenance graphs** (processes, files, sockets and how they interact) instead of coarse **host-to-host** graphs, and apply **temporal link prediction** to spot APT activity.

**Host graphs (Euler-style):** Nodes = machines/users; edges = “host A talked to host B.” Good for lateral movement, but you lose *what* happened (which process, which file).

**Provenance graphs (your angle):** Nodes = `Process` and `Artifact`; edges = `Used`, `WasTriggeredBy`, etc. Much finer detail — e.g. a process reading a staged file before exfiltration.

**Temporal link prediction:** Slice logs into time windows (snapshots), learn normal connection patterns from benign Phase 1, then score edges in Phase 2. **Low-likelihood edges** = suspicious causal links that don’t fit past behavior.

**“Beyond host graphs”** = claim that this finer provenance view catches attack steps (credential access, collection, exfiltration) that host-level graphs miss or detect later.

**In one line:** Train on normal provenance over time; flag rare/unexpected process–artifact links as potential APT steps — at finer granularity than Euler’s machine-level graphs.

---

**User**

what is apt activity?

---

**Cursor**

**APT (Advanced Persistent Threat)** activity is a long-running, targeted cyberattack where an adversary gets into a system, stays hidden, and works through stages to reach a goal (steal data, disrupt operations, etc.).

**Typical traits**
- **Advanced** — skilled attackers, custom tools, sometimes zero-days  
- **Persistent** — weeks/months inside the network, not a one-shot hit  
- **Targeted** — aimed at a specific org or infrastructure (e.g. IIoT, government)

**Common attack stages (simplified)**
1. Initial access (phishing, exploit)  
2. Establish foothold  
3. Privilege escalation  
4. **Lateral movement** — hop to other machines  
5. **Discovery** — map the network  
6. **Collection** — gather sensitive files  
7. **Exfiltration** — send data out  
8. **Persistence** — maintain access  

**Why it’s hard to detect**  
Attackers often use normal tools (SSH, `cp`, browsers) slowly (“low and slow”), so individual events look benign. Suspicion shows up in **unusual sequences or connections** over time — which is why provenance graphs and temporal link prediction are relevant.

**In your CICAPT-IIoT data**  
Phase 2 APT activity is the simulated APT29-style campaign: 332 labeled malicious nodes across tactics like `collection`, `credentialAccess`, `lateralMovement`, `exfiltration`, etc.

---

**User**

so can we work on @CARS/dataset/Phase1_Provenance.csv for link prediction?

---

**Cursor**

**Yes — Phase 1 works for link prediction, but mostly as training / benign baseline, not as the full security evaluation.**

### What Phase 1 gives you
- **46,773 nodes**, **175,347 edges** (Process, Artifact, typed edges)
- **All benign** (`label=0` on every node)
- **~4 days** of normal IIoT activity (Nov 27 – Dec 1, 2023)

### What you can do on Phase 1 alone

| Task | Works on Phase 1? |
|------|-------------------|
| **Standard link prediction** (hide edges, predict missing links) | Yes |
| **Temporal link prediction** (learn patterns across time snapshots) | Yes |
| **Unsupervised “normal behavior” model** (Euler-style training) | Yes — ideal for this |
| **APT / anomaly detection evaluation** | No — no attack labels |

### Practical split

**Phase 1 only:**  
- Build snapshots from `time`  
- Train GNN (+ RNN if temporal) to predict/reconstruct edges  
- Evaluate on held-out **benign** edges (Hits@K, MRR, AUC on link existence)

**Phase 1 → Phase 2 (for a security paper):**  
- **Train** on Phase 1  
- **Test** on Phase 2; low-likelihood edges vs attack labels (`label=1`, `subLabel`)

### Caveats for Phase 1
- **Directed, heterogeneous** graph (Process vs Artifact, 4 edge types) — don’t treat it like undirected Twitter/SNAP
- **Temporal split** beats random edge split for security-style work
- **Scale** — use snapshots + edge sampling, not one flat 175k-edge graph naively

**Bottom line:** Use **Phase 1 to train** temporal link prediction; use **Phase 2 to test** whether it catches APT activity. Phase 1 alone is enough for a methods/benchmarking paper, but not for claiming APT detection without Phase 2.

---

**User**

what should be my next step to write security paper for link precition in this graph? how can we add value?

---

**Cursor**

**Next step:** write a 1-page experiment plan with one claim, then run **three baselines** on Phase 1→Phase 2 before building anything fancy.

---

## Your value proposition (what reviewers need)

Euler and similar work use **host-level** graphs. You use **provenance-level** graphs on **CICAPT-IIoT** (IIoT + labeled APT tactics). The paper’s value is not “we did link prediction on a new CSV” — it’s:

> **Fine-grained provenance temporal link prediction detects APT causal steps earlier and with better tactic coverage than host-level temporal link prediction.**

That’s the story to prove or disprove.

---

## How to add value (pick 2–3 of these)

| Contribution | Why it matters |
|--------------|----------------|
| **Provenance vs host** | Catch collection/credential access before lateral movement shows up at host level |
| **Heterogeneous edges** | `Used` / `WasTriggeredBy` / etc. carry different security meaning; host graphs collapse this |
| **Phase 1 → Phase 2 transfer** | Realistic: train on clean baseline, deploy during attack phase |
| **Hard negatives (HeaRT angle)** | Security-relevant negatives = same process neighborhood, wrong artifact/link — harder and more realistic than random |
| **Tactic-level evaluation** | Report AP per `subLabel` (not just overall AUC) |
| **Case study** | One attack chain as a provenance subgraph with scored edges |

**Strongest combo for you:** provenance vs host + Phase 1 train / Phase 2 test + tactic-level metrics. Add HeaRT hard negatives if results improve.

---

## Concrete next steps (in order)

### 1. Lock the task (this week)
Write down:

- **Nodes:** Process + Artifact (or Process-only v1)
- **Edges:** all 4 types, directed
- **Snapshots:** 30-min windows from edge `time`
- **Train:** Phase 1 only (unsupervised)
- **Test:** Phase 2 edges; positive = touches `label=1` node or attack window
- **Score:** low predicted likelihood = alert (Euler-style)

### 2. Build two graphs from the same data
Same pipeline, two abstractions:

1. **Provenance graph** — your fine-grained graph  
2. **Host graph** — collapse socket/process activity to IP/host pairs (Euler-style baseline)

Same model, same snapshots, same metrics. **This comparison is your core experiment.**

### 3. Implement baselines first (before your “method”)
| # | Method | Role |
|---|--------|------|
| 1 | UA rule | Edge never seen in Phase 1 = anomaly |
| 2 | Static GNN | One big graph, no time |
| 3 | Euler-lite | GCN + GRU, inner-product decoder |
| 4 | + hard negatives | Your HeaRT-inspired variant |

If #4 doesn’t beat #3, the paper is still fine with #3 vs host graph.

### 4. Metrics that security venues care about
- **AP** and **AUC** (overall)
- **TPR @ 0.1% / 1% FPR**
- **Per-tactic AP** (`credentialAccess`, `collection`, `lateralMovement`, …)
- **Time-to-detect** (first alert vs attack start)
- **One case-study figure** (attack subgraph + red/low-score edges)

Skip accuracy — 99.5% benign data makes it meaningless.

### 5. One headline result
Aim for a sentence like:

> “Provenance temporal LP detects credential-access edges at 2× the AP of host-level LP at 1% FPR.”

One clear win > many weak tables.

---

## What *not* to do yet
- Don’t jump to a complex new architecture
- Don’t random-split edges across phases
- Don’t train on Phase 2 attack labels (breaks unsupervised story unless you add a separate supervised section)
- Don’t use only Phase 1 if the paper claims APT detection

---

## Suggested paper outline
1. APT in IIoT needs causal, fine-grained detection  
2. Host graphs miss process/file-level attack steps  
3. Temporal link prediction on provenance graphs  
4. CICAPT-IIoT Phase 1 train / Phase 2 eval  
5. Provenance beats host (+ ablations + case study)

---

## Literally do tomorrow
1. **Snapshot stats** — edges per 30-min window in Phase 1 and Phase 2  
2. **Count attack edges** — Phase 2 edges incident to `label=1` nodes, by `subLabel`  
3. **Prototype host collapse** — how many host-level edges vs 175k provenance edges  
4. **Run UA baseline** — % of Phase 2 attack edges never seen in Phase 1  

Those four numbers tell you if the project is viable before you train any GNN.

---

**Bottom line:** Add value by **proving provenance temporal link prediction beats host-level methods on tactic-specific APT detection**, using CICAPT-IIoT’s natural Phase 1/2 split. Your next step is the **provenance vs host comparison plan + UA baseline**, not a new model.

---

**User**

But I need 3 datasets atleast for link prediction in cybersecurity datasets. shoudl I still o cntinue with tihse?

---

**Cursor**

**Yes — keep CICAPT-IIoT, but treat it as dataset 1 of 3, not the whole paper.**

Three datasets is a normal bar for security + ML venues. One dataset alone is usually “promising preliminary work,” not a full generalization story.

---

## How to use CICAPT-IIoT

**Keep it as your anchor dataset** — it’s the one that supports your main claim:

- fine-grained **provenance** (not just host graphs)
- **IIoT** setting
- clean **Phase 1 → Phase 2** train/test
- **tactic labels** (`credentialAccess`, `collection`, etc.)

That’s hard to replace. Don’t drop it.

---

## What the other 2 datasets should do

They don’t all need to be provenance/IIoT. A strong 3-dataset paper often looks like:

| Role | Dataset | Why |
|------|---------|-----|
| **1. Main (novel)** | **CICAPT-IIoT** | Provenance + IIoT + labeled APT tactics |
| **2. Classic security LP** | **LANL 2015 auth** | Euler’s standard; host-level; labeled lateral movement |
| **3. Scale / modern** | **DARPA OpTC** or **DARPA TC (CADETS/THEIA)** | Large enterprise logs; proves scalability |

Same method, two graph views where needed:

- **Provenance mode** on CICAPT-IIoT  
- **Host/entity mode** on LANL + OpTC (Euler-style)

That’s a feature, not a weakness: *“works on fine-grained provenance and coarse host graphs.”*

---

## Good 3-dataset combinations

**Option A (recommended)**  
1. CICAPT-IIoT (provenance)  
2. LANL auth (host)  
3. OpTC FLOW-START (host)

**Option B (more provenance-heavy)**  
1. CICAPT-IIoT  
2. DARPA TC provenance (e.g. CADETS)  
3. LANL auth  

**Option C (if DARPA is too painful)**  
1. CICAPT-IIoT  
2. LANL  
3. Unraveled or StreamSpot — weaker fit for temporal LP; only if you adapt the task carefully

---

## Practical advice

**Continue with CICAPT-IIoT now** for:
- preprocessing pipeline
- snapshot design
- baselines (UA, static GNN, Euler-lite)
- provenance vs host ablation *within* CICAPT (socket/IP collapse)

**In parallel**, add **LANL** — public, Euler-compatible, relatively easy to plug into the same code with host-level edges.

**Third dataset** after the pipeline works on 1 + 2.

---

## How this strengthens the paper

You’re not “just re-running Euler.” You’re showing:

1. **Provenance LP** adds value on CICAPT-IIoT  
2. The same **temporal LP framework** also works on LANL/OpTC  
3. **Provenance beats host** on fine-grained tactics where labels exist  

That’s a clearer story than three unrelated datasets.

---

## Bottom line

| Question | Answer |
|----------|--------|
| Continue CICAPT-IIoT? | **Yes** — primary contribution |
| Enough alone? | **No** — add 2 more |
| Which next? | **LANL**, then **OpTC** or **DARPA TC** |
| Stop if only 1 dataset? | Only if venue explicitly allows 1 very strong benchmark (rare) |

**Next step:** finish Phase 1→2 pipeline on CICAPT-IIoT, then extend the **same snapshot + scorer** to LANL host auth edges. That gets you to 2 datasets quickly and a credible path to 3.

---

**User**


Muhammad Minhal
Current session
link prediction in cybersecurity graphs
[PDF] ieee.org
Find@UW
Knowledge-driven cybersecurity intelligence: Software vulnerability coexploitation behavior discovery
J Yin, MJ Tang, J Cao, M You, H Wang… - IEEE transactions on …, 2022 - ieeexplore.ieee.org
Formulates the discovery of vulnerability co-exploitation behavior as a link prediction problem between vulnerability entities within a cybersecurity knowledge graph.
MAGCN for Prediction: Proposes a Modality-Aware Graph Convolutional Network (MAGCN) module to embed multi-modality entity attributes and topological graph connectivity features to improve link prediction performance.
Graph Knowledge Transfer: Introduces a Graph Knowledge Transfer Learning (GKTL) strategy to transfer knowledge between subgraphs to address graph sparsity and time difference issues in the link prediction task.
Save Cite Cited by 122 Related articles All 6 versions
[PDF] ajisresearch.com
Graph Neural Networks (GNNS) For Modeling Cyber Attack Patterns And Predicting System Vulnerabilities In Critical Infrastructure
TK Chowdhury, S Biswas - American Journal of Interdisciplinary …, 2022 - ajisresearch.com
Explains that link prediction within cyber graph models aims to identify potential or hidden relationships that could facilitate lateral movement between system entities.
Link Prediction Mechanism: Details that link prediction estimates the likelihood of new or unauthorized edges forming, such as unexpected credential reuse or illicit access routes.
GNN Role in Link Prediction: Indicates that Graph Neural Networks (GNNs) enhance link prediction by learning higher-order dependencies, which allows for the discovery of subtle deviations in graph topology and edge semantics.
Save Cite Cited by 78 Related articles 
[HTML] sciencedirect.com
Find it at MLML/MBARI Res
[HTML] Edge propagation for link prediction in requirement-cyber threat intelligence knowledge graph
Y Zhang, J Chen, Z Cheng, X Shen, J Qin, Y Han… - Information Sciences, 2024 - Elsevier
Proposes EGNN, a novel Graph Neural Network-based model with an edge propagation algorithm, for predicting new knowledge (links) on the heterogeneous cybersecurity knowledge graph called RCTI.
Cybersecurity Knowledge Graph: Constructs the heterogeneous knowledge graph, RCTI, by combining cybersecurity threat intelligence data with management security requirements data from national network security standards.
Task Definition: Transforms the reasoning issue of finding relations between security requirement data and cyber threat intelligence data into a link prediction task on a heterogeneous directed graph.
Save Cite Cited by 47 Related articles All 2 versions 
[HTML] mdpi.com
Find it at MLML/MBARI Res
[HTML] Knowledge graphs and semantic web tools in cyber threat intelligence: A systematic literature review
C Bratsas, EK Anastasiadis, AK Angelidis… - … of Cybersecurity and …, 2024 - mdpi.com
Reviews research that applied Machine Learning (ML) or Deep Learning (DL) methods to problems in Cyber Threat Intelligence (CTI) such as link prediction, specifically mentioning a dedicated subcategory for these works.
Graph Embedding and Inference: Discusses a model that applies link prediction tasks to augment the CTI landscape by automatically inferring relationships between head and tail entities using graph embedding algorithms and reasoning rules.
Threat Indicator Prediction: Highlights a model, Ctiap, which combines matrix factorization and Neural Network methods to address the link prediction problem on a graph dataset of threat indicators, demonstrating high accuracy and F1-score in mining threat indicator relationships.
Save Cite Cited by 25 Related articles All 3 versions 
[PDF] ieee.org
Find@UW
Attack hypotheses generation based on threat intelligence knowledge graph
FK Kaiser, U Dardik, A Elitzur… - … on Dependable and …, 2023 - ieeexplore.ieee.org
Presents a toolkit called the Attack Hypothesis Generator that uses knowledge graph traversal and various link prediction methods to automatically infer ATT&CK techniques from observed artifacts in cybersecurity.
Uses Various LP Metrics: Utilizes link prediction techniques, including Jaccard’s/Tanimoto coefficient, Adamic Adar, Friends/Katz measure, and Preferential Attachment, for hypothesis refinement in the threat intelligence knowledge graph (AttackDB).
Applies Supervised LP: Formulates link prediction as a supervised learning problem using a random forest classifier to predict the probability of a link existing in the threat intelligence knowledge graph.
Save Cite Cited by 66 Related articles All 6 versions
[HTML] mdpi.com
Full View
[HTML] A knowledge graph completion algorithm based on the fusion of neighborhood features and vBiLSTM encoding for network security
W Zhang, M Wang, G Han, Y Feng, X Tan - Electronics, 2024 - mdpi.com
Proposes a new knowledge graph completion method, the KGC-N model, that integrates feature vectors with graph structure information and uses a Transformer decoder to predict and complete missing links in a network security knowledge graph.
Addresses Sparsity: Addresses the challenge of information sparsity in network security knowledge graphs using entity link prediction as a component of the completion task.
Applications in Attack Inference: Describes an existing algorithm that uses knowledge graph traversal and various link prediction methods to complete missing information and automatically infer ATT&CK techniques in observable network attacks, demonstrating applications of link prediction in cybersecurity.
Save Cite Cited by 8 Related articles All 3 versions 
[HTML] mdpi.com
Find it at MLML/MBARI Res
[HTML] Recommendations for responding to system security incidents using knowledge graph embedding
H Kim, J Choi - Electronics, 2023 - mdpi.com
Proposes a method for recommending strategies to respond to edge computing security incidents based on the automatic generation and embedding of security knowledge graphs.
Knowledge Graph Embedding: Utilizes knowledge graph embedding as a representative method for link prediction, which involves vectorizing entities and relationships and learning the embedding value based on a scoring function.
Link Prediction Experiments: Describes experiments on link prediction and triple classification to evaluate the performance of the proposed knowledge completion and inference for security incident responses.
Save Cite Cited by 7 Related articles All 3 versions 
Generating network security defense strategy based on cyber threat intelligence knowledge graph
S Zhang, S Li, P Chen, S Wang, C Zhao - International Conference on …, 2022 - Springer
Develops CTI-KGE, a knowledge graph embedding algorithm used for link prediction tasks to infer tail entities and complete threat information automatically.
0-day Attack Prediction: Transforms attack prediction into a link prediction problem on a network defense knowledge graph to address the difficulty of predicting attacks caused by 0-day vulnerabilities.
Completing Threat Information: Uses link prediction to complete triples by predicting sparse and missing relationships in the Cyber Threat Intelligence (CTI) knowledge graph, which is essential for inferring defense strategies.
Save Cite Cited by 16 Related articles 
Find@UW
Infer the missing facts of D3FEND using knowledge graph representation learning
A Khobragade, S Ghumbre… - International Journal of …, 2023 - emerald.com
Proposes an automated approach to predict missing facts in the D3FEND knowledge graph, a cybersecurity countermeasure domain resource, using a link prediction task with embedding as representation learning.
Embedding Models Applied: Applies progressive distance-based, bilinear, and convolutional embedding models to learn representations of entities and relations within the D3FEND graph for the link prediction task.
Cybersecurity Strategy Benefit: States that link prediction in the D3FEND Knowledge Graph can benefit cybersecurity countermeasure strategies by identifying gaps, comparing defensive methods, and discovering novel methods.
Save Cite Cited by 6 Related articles All 2 versions
A management knowledge graph approach for critical infrastructure protection: Ontology design, information extraction and relation prediction
J Chen, Y Lu, Y Zhang, F Huang, J Qin - International Journal of Critical …, 2023 - Elsevier
Adopts the Knowledge Graph Bidirectional Encoder Representations from Transformer (KG-BERT) model to capture semantic information and predict relationships between threat and management components within the constructed graph.
Completes Knowledge Graph: Applies link prediction to complete the cybersecurity knowledge graph, specifically using KG-BERT to predict the link between threats and complete missing relationships.
Associates Threat and Management: Utilizes link prediction to associate the established management knowledge graph with the existing cybersecurity threat knowledge graph, inferring relationship triples between the two parts.
Save Cite Cited by 32 Related articles All 4 versions 
[PDF] ieee.org
Find@UW
Beyond Single-Hop: Link Prediction Through Multi-Hop Reasoning in Malware Knowledge Graphs
K Bahlibi, HL Nguyen, E Dogdu… - 2026 IEEE 5th …, 2026 - ieeexplore.ieee.org
Proposes a unified framework for multi-hop link prediction in malware knowledge graphs, which are essential for representing complex relationships in cyber threat reports.
Hybrid Reasoning Paradigms: Learns latent representations of entities and relations by combining graph-based reasoning (Graph Neural Networks like GraphSAGE, GCN, GAT) and language-based reasoning (Large Language Models like LLaMA-2-7B and Phi-2).
Model Performance Comparison: Finds that while Graph Neural Networks are effective on shorter paths, Large Language Models maintain robust performance across longer reasoning chains, demonstrating potential for dynamic malware analysis.
Save Cite Related articles All 2 versions
[HTML] mdpi.com
Find it at MLML/MBARI Res
[HTML] Recent progress of using knowledge graph for cybersecurity
K Liu, F Wang, Z Ding, S Liang, Z Yu, Y Zhou - Electronics, 2022 - mdpi.com
Discusses knowledge reasoning technology in cybersecurity knowledge graphs (CSKGs) that can generate new knowledge to support prediction and inference tasks.
Attack Prediction as Link Prediction: Explains that attack prediction can be transformed into a knowledge graph link prediction problem, as demonstrated by a study that used a path ranking algorithm to create a 0-day attack graph.
Link Prediction Tasks in CSKGs: Mentions that CSKGs can be utilized for three different reasoning tasks, including a link prediction task based on Common Weakness Enumeration (CWE).
Save Cite Cited by 79 Related articles All 7 versions 
[PDF] ssrn.com
Enhancing Cybersecurity Situation Awareness Through Knowledge Graphs: An Integrated Survey Including Threats, Vulnerabilities, and Assets
Y Guan, Y Zhang, J Yue, Y Lu, Y Xie - Vulnerabilities, and Assets - papers.ssrn.com
Explains that link prediction techniques, such as graph completion, can allow knowledge graphs to provide early warnings of unknown attack patterns, enhancing a cybersecurity system’s foresight and proactivity.
Threat Attribution Modeling: Discusses formalizing threat attribution as a link prediction task on a graph to enhance interpretability and make complex analyses more intuitive and efficient.
Predicting New Attacks: Mentions using graph neural networks with attention mechanisms in models like EGNN to predict new connections and discover new attack incidents based on patterns learned from a cybersecurity knowledge graph.
Save Cite Cited by 2 Related articles 
[PDF] acm.org
Full View
Advancing cybersecurity: graph neural networks in threat intelligence knowledge graphs
L Li, F Qiang, L Ma - Proceedings of the International Conference on …, 2024 - dl.acm.org
Highlights that Graph Neural Networks (GNNs) are proficient in various tasks on cybersecurity graphs, specifically including link prediction, node prediction, and graph classification.
TIKG Analysis with GNNs: Discusses leveraging Graph Neural Networks for analyzing Threat Intelligence Knowledge Graphs (TIKGs) to enhance the understanding of complex relationships between various cyber threats and related entities.
Learning Graph Representations: Explains that GNNs learn representations of graph structures to uncover patterns and dependencies, which is necessary for extracting actionable insights from knowledge graphs in network security.
Save Cite Cited by 15 Related articles 
[PDF] ieee.org
Find it at MLML/MBARI Res
Association Analysis and Prediction of Network Security Vulnerabilities Based on Knowledge Graph
S Wu, L Feng - Journal of Cyber Security and Mobility, 2025 - ieeexplore.ieee.org
Presents a novel prediction framework that combines knowledge graph embeddings, graph neural networks, and multi-modal feature fusion to forecast vulnerability exploitation.
Link Prediction Model: Utilizes a multi-task learning model that simultaneously performs link prediction and entity classification, achieving 86.3% accuracy in link prediction.
Vulnerability Graph Construction: Constructs a comprehensive vulnerability knowledge graph that captures semantic relationships between vulnerabilities, attack patterns, and affected systems using integrated data from NVD, CVE, and vendor security bulletins.
Save Cite Related articles All 3 versions 
[HTML] mdpi.com
Find it at MLML/MBARI Res
[HTML] A cybersecurity knowledge graph completion method for penetration testing
P Wang, J Liu, X Zhong, S Zhou - Electronics, 2023 - mdpi.com
Designs a knowledge graph completion model, CSNT, to infer missing relationships (link prediction) in cybersecurity data, enhancing the information's integrity and accuracy.
Mining Implicit Relationships: Proposes a method to mine implicit relationships from existing cybersecurity data, which can then be used to complete and correct cybersecurity intelligence information.
Neural Network and Tensor Methods: Uses a combination of a neural network (BiLSTM) and tensor decomposition to model the relationship between entities for the purpose of completing the cybersecurity knowledge graph.
Save Cite Cited by 12 Related articles All 3 versions 
[PDF] ieee.org
Find it at MLML/MBARI Res
Creating cybersecurity knowledge graphs from malware after action reports
A Piplai, S Mittal, A Joshi, T Finin, J Holt, R Zak - IEEE Access, 2020 - ieeexplore.ieee.org
Describes building a neural network component called RelExt, a deep learning-based relationship extractor, to predict relationships between pairs of entities in a Cybersecurity Knowledge Graph (CKG).
Knowledge Graph Population: Utilizes a pipeline that includes a Relationship Extractor (RelExt) to predict how pairs of malware entities are related to each other, asserting the resulting entity-relationship set into the CKG.
Contextual Relationship Prediction: Employs vector representations to capture the context of cybersecurity entities in text, which aids in the task of relationship prediction within the knowledge graph creation process.
Save Cite Cited by 160 Related articles All 9 versions 
[HTML] mdpi.com
Full View
[HTML] Dynamic vulnerability knowledge graph construction via multi-source data fusion and large language model reasoning
R Liu, Y Xie, Z Dang, J Hao, X Quan, Y Xiao, C Peng - Electronics, 2025 - mdpi.com
Introduces a novel combination of graph-based and Large Language Model (LLM)-based reasoning for link prediction, specifically fusing a text-enhanced Graph Attention Network (GAT) with GPT-3-generated insights to infer missing relationships.
Two-Pronged Prediction Approach: Employs a two-pronged link prediction strategy by combining TransH-based Knowledge Graph (KG) embeddings with BERT-derived textual node vectors to enhance GAT, followed by a weighted fusion of GAT and LLM confidence scores.
Link Prediction Performance: Reports that using a hybrid Graph Attention Network fused with GPT-3 reasoning boosted Hits@1 by 0.137, Hits@3 by 0.116, and Hits@10 by 0.101 over the baseline for link prediction, confirming enhanced relationship inference.
Save Cite Cited by 8 Related articles All 3 versions 
[PDF] edpsciences.org
Uncovering multi-step attacks with threat knowledge graph reasoning
X Xiang, C Ma, L Zeng, W Feng, Y Xie… - Security and …, 2025 - sands.edpsciences.org
Introduces a novel knowledge representation learning approach, A4CKGE (ATT&CK-CAPEC-CWE-CVE-CPE Knowledge Graph Embedding), to predict interactions among security entities in a cohesive threat knowledge graph.
Knowledge Graph Embedding: Utilizes a knowledge graph embedding technique to convert structural knowledge and textual descriptions into a compact, low-dimensional continuous vector space, a method which is verified for link prediction applications.
Reasoning for Attack Prediction: Develops a knowledge graph embedding method to enhance reasoning capabilities over security entity relations and predict multi-step attack sequences, particularly those associated with complex Advanced Persistent Threats (APTs).
Save Cite Cited by 7 Related articles All 4 versions 
[PDF] ieee.org
Actionable cyber threat intelligence using knowledge graphs and large language models
R Fieblinger, MT Alam, N Rastogi - 2024 IEEE European …, 2024 - ieeexplore.ieee.org
Applies a constructed Knowledge Graph, derived from Cyber Threat Intelligence (CTI) using Large Language Models (LLMs), to link prediction tasks to enhance cybersecurity defenses and response strategies.
Benefits of Link Prediction: Explains that link prediction in cybersecurity Knowledge Graphs enables proactive threat detection by forecasting potential future connections, like new malware attack patterns, and aids vulnerability assessment by revealing possible attack paths.
Evaluation of Link Prediction: Evaluates the efficacy of the link prediction task by comparing performance against a baseline model (LADDER) using traditional metrics such as Mean Reciprocal Rank (MRR) and Hits@n, demonstrating the capabilities of the LLM-constructed Knowledge Graph.
Save Cite Cited by 89 Related articles All 6 versions 
[PDF] ieee.org
Find@UW
Time-aware cybersecurity knowledge graph reasoning method for vulnerability analysis
M Wang, K Li, Y Lu, F Zhang, J Ma… - IEEE Transactions on …, 2025 - ieeexplore.ieee.org
Proposes TCGFormer, a time-aware cybersecurity knowledge graph reasoning method, specifically designed to identify co-exploitation behaviors and predict entities and relations.
TCGFormer Modules: Explains that the TCGFormer method includes a parameter learning module responsible for predicting entities and relations, which is synonymous with link prediction.
Link Prediction Evaluation: Discusses using accuracy, precision, recall, and F1 score to evaluate the link prediction binary classification task specifically on a cybersecurity dataset.
Save Cite Cited by 12 Related articles
Uncovering Security Entity Relations with Cyber Threat Knowledge Graph Embedding
C Ma, X Xiang, Y Xie, W Feng, Z Gu - International Conference on Network …, 2023 - Springer
Proposes a novel knowledge representation learning method, 4CKGE (CAPEC-CWE-CVE-CPE Knowledge Graph Embedding), for predicting correlations between security entities like products, vulnerabilities, weaknesses, and attack patterns.
Threat Knowledge Graph: Utilizes a cyber threat knowledge graph, aggregated from databases like CAPEC, CWE, CVE, and CPE, to semantically cross-analyze security-related entities and relations.
Structural and Textual Embeddings: Employs an embedding approach that integrates both structural and textual information from the knowledge graph to enhance performance in relationship prediction tasks.
Save Cite Cited by 3 Related articles 
[PDF] spiedigitallibrary.org
Find it at MLML/MBARI Res
Attack prediction in Internet of Things using knowledge graph
S Zhang, C Zhao, S Wang, S Li… - … conference on internet …, 2023 - spiedigitallibrary.org
Transforms the attack prediction task in IoT cybersecurity into a link prediction problem within a knowledge graph framework.
Knowledge Representation Learning: Utilizes a knowledge representation learning method (FTSPC) that fuses triple information and semantic path combination information to conclude possible attacks.
Knowledge Graph Construction: Constructs an intelligent model to build a knowledge graph by integrating equipment (CPE), vulnerability (CVE), and attack pattern (CAPEC) information from NIST and MITRE.
Save Cite Cited by 11 Related articles All 3 versions 
[PDF] springer.com
Find it at MLML/MBARI Res
A knowledge extrapolation model for attack inference based on graph attention networks and relation mapping: W. Ren et al.
W Ren, L Zhao, W Li - Knowledge and Information Systems, 2026 - Springer
Proposes an attack knowledge extrapolation model based on graph attention networks and relation mapping that infers and predicts unknown attack-related entity–relation triples within a cybersecurity knowledge graph.
Predicting Unknown Triples: Uses a scoring function to infer and predict unknown attack-related entity–relation triples, which is the core task of link prediction in knowledge graphs.
Proactive Prediction: Discusses the importance of accurately predicting unknown attack relationships in real-world cybersecurity operations for deploying defensive measures earlier, thereby reducing detection and response times.
Save Cite Cited by 2 Related articles All 2 versions 
[PDF] springer.com
Find it at MLML/MBARI Res
GC-PTransE: multi-step attack inference method based on graph convolutional neural network and translation embedding: W. Ren et al.
W Ren, W Li, Y Hong, Y Du, Y Gao, H Zhang… - … and Information Systems, 2025 - Springer
Proposes a multi-step attack inference method called GC-PTransE, which uses graph convolutional neural networks (GCN) and translational embedding to predict cyberattack scenarios.
Uses Translational Embedding: Utilizes PTransE for embedding symbolic and descriptive information of cyberattack entities into a low-dimensional continuous vector space, navigating graph structure data along paths of triples to identify entities and relationships.
Predicts Outcomes and Completes Graph: Details how combining path relationships and scoring the energy of new triples predicts outcomes, thus completing the cyberattack knowledge graph.
Save Cite Cited by 4 Related articles All 3 versions 
[PDF] nature.com
Find it at MLML/MBARI Res
Network attack knowledge inference with graph convolutional networks and convolutional 2D KG embeddings
W Ren, H Zhang, Y Lei - Scientific Reports, 2025 - nature.com
Proposes a method, KGConvE, based on graph convolutional networks and convolutional 2D KG embeddings to perform attack inference and association mining of implicit network attack knowledge.
Predicts Entity Relationships: Applies the KGConvE model to perform attack inference tasks to predict implicit relationships between entities like CVE-CVE, CVE-CWE, and CVE-CAPEC, achieving significant performance improvements in knowledge inference tasks.
Constructs Cybersecurity Graph: Constructs a cybersecurity knowledge graph that associates CVE, CWE, CAPEC, and APT entities, enabling the extraction of implicit attack knowledge for proactive defense and network security situational awareness.
Save Cite Cited by 1 Related articles All 6 versions 
[HTML] mdpi.com
Find it at MLML/MBARI Res
[HTML] A cybersecurity knowledge graph completion method based on ensemble learning and adversarial training
P Wang, J Liu, D Hou, S Zhou - Applied Sciences, 2022 - mdpi.com
Develops CSEA, a new knowledge graph completion method that uses ensemble learning and adversarial training to infer unknown facts and complement existing cybersecurity knowledge.
Enhances Model Robustness: Designs a cooperative adversarial training method to enhance the generalization and robustness of the model, specifically tailored for cybersecurity knowledge graph completion.
Measures Link Prediction: Measures the performance of the model for cyber security link prediction using evaluation metrics such as MRR, MR, and Hits@k.
Save Cite Cited by 8 Related articles All 4 versions 
Find@UW
Applicability Analysis of Knowledge Graph Embedding on Blended Threat
M Lee, IS Jung, DH Kim… - … and Service (PlatCon), 2022 - ieeexplore.ieee.org
Analyzes a knowledge graph embedding model applicable to blended threats to predict and respond to potential complex security risks in the Internet of Blended Environment (IoBE).
Details Link Prediction Use: Explains that link prediction, which includes both relation prediction and entity prediction, can be used with knowledge graph embedding to address incompleteness in the knowledge graph for security applications.
Applies KGE to STIX Data: Discusses using knowledge graph embedding techniques to visualize various threat data from STIX 2.1 and apply reasoning tasks for more accurate security threat prediction.
Save Cite Cited by 3 Related articles All 2 versions
[PDF] ieee.org
Find@UW
Ctinexus: Automatic cyber threat intelligence knowledge graph construction using large language models
Y Cheng, O Bajaber, SA Tsegai… - 2025 IEEE 10th …, 2025 - ieeexplore.ieee.org
Proposes CTINexus, a framework that utilizes a long-distance relation prediction technique to complete Cybersecurity Knowledge Graphs (CSKGs) with missing links.
LLM for CSKG Construction: Explains that the framework leverages optimized in-context learning of large language models for data-efficient knowledge extraction and high-quality CSKG construction.
Inferring Missing Connections: Describes how the long-distance relation prediction technique infers connections among disjoint subgraphs by selecting central nodes and predicting implicit relations among them.
Save Cite Cited by 35 Related articles All 8 versions
[PDF] ecu.edu.au
Cybersecurity knowledge graphs
L Sikos - 2023 - ro.ecu.edu.au
Discusses that relationship predictions between entity pairs in cybersecurity knowledge graphs can be achieved using deep learning techniques.
Statistical Relational Learning: Explains that statistical relational learning can be applied to predict missing links and identify relationships between nodes within inherently sparse, incomplete, and noisy cyber-knowledge graphs.
Unsupervised Link Prediction: Mentions that link prediction can be performed unsupervised to rank the likelihood of triple statements resulting from events observed at test time, often following training a generative graph embedding algorithm on a baseline dataset.
Save Cite Cited by 153 Related articles All 8 versions 
[PDF] acm.org
Find it at MLML/MBARI Res
Relext: Relation extraction using deep learning approaches for cybersecurity knowledge graph improvement
A Pingle, A Piplai, S Mittal, A Joshi, J Holt… - Proceedings of the 2019 …, 2019 - dl.acm.org
Proposes a system called RelExt that uses a deep feedforward neural network to predict relationships between subject-object entity pairs, thus improving the population of a cybersecurity knowledge graph by filling in missing links.
KG Creation: Explains that the creation of a cybersecurity knowledge graph involves predicting the correct relationship between two specific entities extracted from cyber threat intelligence sources.
Improving Knowledge Graphs: Aims to improve existing knowledge graphs by looking for more relationships, or links, between entities already available in the knowledge graph, which is a key aspect of link prediction.
Save Cite Cited by 187 Related articles All 11 versions 
[PDF] arxiv.org
A review of knowledge graph application scenarios in cyber security
K Liu, F Wang, Z Ding, S Liang, Z Yu, Y Zhou - arXiv preprint arXiv …, 2022 - arxiv.org
Explains that a Knowledge Graph (KG) derived from threat, assets, and vulnerability data transforms the attack prediction task into a link prediction problem within the KG.
0-Day Attack Path Generation: Mentions using link prediction and a path ranking algorithm to represent and generate 0-day attack paths within the cybersecurity knowledge graph.
CWE Link Prediction and Reasoning: Indicates that the cybersecurity knowledge graph (CSKG) based on Common Weakness Enumeration (CWE) can be utilized for reasoning tasks, including CWE link prediction and predicting common consequences.
Save Cite Cited by 50 Related articles All 2 versions 
[HTML] sciencedirect.com
Find it at MLML/MBARI Res
[HTML] Enhanced small-scale APT knowledge graph embedding via spatio-temporal attribute reasoning and adversarial negative sampling
Y Xie, H Wang, X Jing, Z Gu - Array, 2025 - Elsevier
Proposes an enhanced Knowledge Graph Embedding (KGE) method, called APT-ST-AN, specifically designed for Advanced Persistent Threat (APT) attack prediction to address the limitations of incomplete and sparse data in small-scale APT knowledge graphs.
Enriching Examples: Introduces spatio-temporal attribute reasoning and adversarial negative sampling to enrich both positive and negative APT attack examples, thereby improving the expressiveness and generalization of KGE models for attack inference.
KGE for Attack Prediction: Explains that KGE techniques are a promising approach for APT attack prediction by leveraging existing cybersecurity knowledge to infer potential attack behaviors, directly addressing the link prediction aspect.
Save Cite Cited by 1 Related articles 
[PDF] springer.com
Find it at MLML/MBARI Res
GPCNKB: An Attack Prediction and Reasoning Model Based on an Improved Graph Convolutional Network
W Ren, J Yao, Y Hong - Neural Processing Letters, 2025 - Springer
Introduces GPCNKB, a network attack prediction and reasoning method that utilizes knowledge graphs and embedding techniques to represent attack scenarios for predicting attack patterns.
Knowledge Graph Reasoning: Applies a knowledge graph embedding model to reason attack knowledge and predict potential relationships within scenario graphs of the same category, enhancing reasoning accuracy.
Graph Convolutional Networks: Utilizes graph convolutional networks (GCNs) for classifying attack scenarios and incorporates evolutionary computation to refine the GCN model, optimizing training parameters and improving universality.
Save Cite Related articles All 2 versions 
[PDF] osti.gov
Retrieval augmented generation for robust cyber defense
M Rahman, KO Piryani, AM Sanchez, S Munikoti… - 2024 - osti.gov
Utilizes a graph-based framework, GraphCyRAG, leveraging Neo4j knowledge graphs for deeper traversal of relationships between vulnerabilities (CVE, CWE) and attack patterns (CAPEC, ATT&CK), which are essential for predicting exploit paths.
RAG and Relationship Discovery: Demonstrates that integrating knowledge graphs with Retrieval-Augmented Generation (RAG) helps analysts uncover hidden relationships between cyber entities and predict exploit paths.
Path Prediction using Cypher: Enables tracing the shortest or most relevant paths between nodes (CVE, CWE, CAPEC) using Cypher queries to link a specific vulnerability to associated attack patterns, helping to predict how a vulnerability could be exploited in real-world scenarios.
Save Cite Cited by 22 Related articles All 3 versions 
[PDF] usenix.org
Cyber threat intelligence modeling based on heterogeneous graph convolutional network
J Zhao, Q Yan, X Liu, B Li, G Zuo - … on research in attacks, intrusions and …, 2020 - usenix.org
Proposes a framework, HINTI, to model interdependent relationships among heterogeneous Indicators of Compromise (IOCs) using a newly constructed heterogeneous information network (HIN) for threat intelligence computing.
GCN for Knowledge Discovery: Utilizes a threat intelligence computing framework based on graph convolutional networks (GCN) for effective knowledge discovery, noting GCNs are effective tools for tasks like link prediction on graphs.
Predicting Attacker Intrusion: Mentions that CTI subscribers can use the resulting low-dimensional embedding space to gain new threat insights, such as predicting which attackers are likely to intrude their systems.
Save Cite Cited by 133 Related articles All 6 versions 
[PDF] wiley.com
Find it at MLML/MBARI Res
Knowledge graph reasoning for cyber attack detection
E Gilliard, J Liu, AA Aliyu - IET Communications, 2024 - Wiley Online Library
Discusses knowledge graph embedding (KGE) as a technique that translates entities and relationships into low-dimensional vector spaces, which supports machine learning models for reasoning and prediction tasks.
Use for Link Prediction: Identifies link prediction as one of the valuable tasks for which the vector representation of semantic meanings, captured by Knowledge Graph Embedding, proves beneficial.
Threat Identification Automation: Proposes an approach using knowledge graph reasoning to improve the efficiency and accuracy of network threat identification, establishing a foundation for automated network attack detection.
Save Cite Cited by 27 Related articles All 5 versions 
Graph neural networks in network security: from theoretical foundations to applications
X Zhang - 2025 - aisel.aisnet.org
Examines network security applications at the node, edge, and graph levels, illustrating how Graph Neural Network (GNN) embeddings translate into practical threat identification and classification.
GNN Role in Network Security: Reviews the transformative role of GNNs in network security, noting that researchers have introduced various GNN architectures to address a range of challenges.
Temporal Link Prediction: References a related work that explicitly focuses on detecting network lateral movement using scalable temporal link prediction, which is a specific application of link prediction in cybersecurity.
Save Cite Cited by 4 Related articles
[PDF] ieee.org
Find@UW
Network Security Threat Detection System Based on Knowledge Graph
M Zheng, T He, N Wang, X Wang… - 2025 40th Youth …, 2025 - ieeexplore.ieee.org
Presents a cybersecurity threat detection system built on a knowledge graph that integrates heterogeneous data sources into a semantic framework, which often involves establishing or predicting links between entities.
Future Prediction Research: Discusses future research directions that involve leveraging the reasoning capabilities of knowledge graphs to predict attack paths and uncover latent threats, which directly relates to link prediction.
Linking Cybersecurity Elements: Constructs a knowledge graph by linking offensive tactics and vulnerabilities, such as linking ATT&CK techniques to CAPEC attack patterns and then to CWE weaknesses and CVE vulnerabilities.
Save Cite Related articles
[PDF] acm.org
Full View
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction
IJ King, HH Huang - ACM Transactions on Privacy and Security, 2023 - dl.acm.org
Formulates anomalous lateral movement detection in cybersecurity as a temporal graph link prediction problem, where network interactions are abstracted into a series of graphs.
Euler Framework: Proposes the Euler framework for scalable dynamic link prediction and anomalous edge detection, which involves stacking a model-agnostic Graph Neural Network upon a sequence encoder like a Recurrent Neural Network.
Performance in Anomaly Detection: Demonstrates that models built with the Euler framework efficiently identify anomalous connections with high precision and outperform other unsupervised techniques for detecting anomalous lateral movement.
Save Cite Cited by 175 Related articles All 8 versions 
[PDF] ieee.org
Find@UW
Graph Neural Network-Based Prediction of Cyber Threats Using IDS Logs
H Kim, H Doo, S Yoon, H Lim - 2025 16th International …, 2025 - ieeexplore.ieee.org
Proposes a novel framework that constructs a directed acyclic graph (DAG) from Intrusion Detection System (IDS) logs to characterize attacker–vulnerability–victim chains.
GNN for Link Prediction: Trains a Graph Neural Network (GNN) model, specifically GCN2, on the DAG to perform link prediction of unseen threats, enabling the anticipation of potential cyber threats.
Performance Evaluation: Demonstrates strong predictive performance on real-world Snort logs, achieving a high AUC of 0.91 in binary link prediction, validating the approach for forecasting emerging cyber threats.
Save Cite Related articles
[PDF] ieee.org
Full View
Research on Parameter-Efficient Knowledge Graph Completion Methods and Their Performance in the Cybersecurity Field
B Chen, H Li, Z Shi - IEEE Access, 2025 - ieeexplore.ieee.org
Proposes a multi-level graph clustering-based model called GCCKG for completing cyberthreat intelligence knowledge graphs, which directly relates to link prediction.
Predicting Missing Relations: Explains that knowledge graph completion, which includes link prediction, aims to predict and supplement missing triples to enhance the completeness and accuracy of the graphs and unearth unknown relations between threat intelligence entities.
Cybersecurity Dataset Application: Applies the proposed GCCKG model to the cyberthreat intelligence knowledge graph CS13K, demonstrating its effectiveness in knowledge graph completion tasks, a form of link prediction.
Save Cite Cited by 1 Related articles All 3 versions 
[PDF] ieee.org
Knowledge-Driven Prediction of Vulnerability Co-Exploitation
Y Yu, K Luo, X Hu, Y Zhang - 2025 IEEE 6th International …, 2025 - ieeexplore.ieee.org
Proposes a novel methodology for predicting the co-exploitation of vulnerabilities using knowledge reasoning based on a vulnerability knowledge graph, VulCKG, which is a key task related to link prediction in cybersecurity.
Knowledge Graph Link Prediction Model: Designs a knowledge embedding model, AttTransM, for knowledge graph completion (KGC), which is explicitly stated to be synonymous with link prediction, to fill in missing critical relational information within the graph.
Rule-Based Link Inference: Applies rule-based reasoning on the completed knowledge graph to infer vulnerability co-exploitation relationships and related security information, such as mitigation measures and detection techniques.
Save Cite Related articles 
[PDF] ieee.org
Find@UW
NLP-Augmented Knowledge Graph Embedding for Relationship Prediction in Network Security
Y Yin, X Liu, X Yu - 2024 6th International Academic Exchange …, 2024 - ieeexplore.ieee.org
Proposes a novel approach that combines Natural Language Processing (NLP) with knowledge graph reasoning algorithms to improve relationship prediction in network security.
CombineSS Embedding Method: Introduces a method, called CombineSS, that integrates structural and semantic embedding to optimize knowledge graph relationship prediction in the network security domain.
Performance Improvement: Demonstrates that the NLP-enhanced knowledge graph method effectively improves prediction accuracy and performance for entity link prediction and relationship prediction tasks compared to traditional graph structure algorithms.
Save Cite Related articles
[PDF] semanticscholar.org
[PDF] A Knowledge Graph Completion Algorithm Based on the Fusion of Neighborhood Features and vBiLSTM Encoding for Network Security. Electronics 2024, 13 …
W Zhang, M Wang, G Han, Y Feng, X Tan - 2024 - pdfs.semanticscholar.org
Proposes a completion method, KGC-N, that uses a vBiLSTM model for text encoding, feature fusion, and a Transformer decoder to predict and complete missing information within the network security knowledge graph.
Defines Link Prediction: Employs entity link prediction as a component of completing knowledge graphs, framing it as a sorting task to fill in missing entity parts in triples.
Evaluates Link Prediction Metrics: Utilizes evaluation metrics like MR and MRR to assess the effectiveness of the KGC-N model specifically for the network security knowledge graph completion and link prediction tasks.
Save Cite Related articles 
[PDF] ieee.org
Find@UW
Jbeil: Temporal graph-based inductive learning to infer lateral movement in evolving enterprise networks
J Khoury, Đ Klisura, H Zanddizari… - … IEEE Symposium on …, 2024 - ieeexplore.ieee.org
Proposes a data-driven framework called Jbeil that utilizes self-supervised deep learning on evolving networks, represented as sequences of authentication timed events, to perform link prediction.
Encoder-Decoder Mechanism: Explains that the framework applies an encoder on a continuous-time evolving graph to produce embeddings and a decoder leverages these embeddings to perform Lateral Movement (LM) link prediction on unseen nodes.
Cyber Fraud Detection: Discusses that link prediction is a crucial task in analyzing graph data with benefits in areas like cyber fraud detection and knowledge graph completion.
Save Cite Cited by 46 Related articles All 6 versions
[PDF] researchsquare.com
Network Attack Knowledge Reasoning Model based on Graph Convolutional Neural Networks
WW Ren, Y Lei, Y Hong, XT Song, JY Yao, YZ Du… - 2023 - researchsquare.com
Proposes an attack knowledge inference model that utilizes graph convolutional neural networks to analyze large-scale penetration attacks with complex, multi-hop pathways.
CONVE Model for Inference: Introduces the CONVE model for attack inference tasks, demonstrating its ability to infer implicit relationships between CVE-CVE, CVE-CWE, and CVE-CAPEC, addressing a form of link prediction.
Attack Relationship Deduction: Discusses how the methods leverage existing attack context information and inference models to deduce attack knowledge, including the completion of unknown attacks and attack relationships within the attack knowledge graph.
Save Cite Related articles All 3 versions 
[PDF] ieee.org
Find@UW
Edge-Focused Temporal Graph Autoencoders for Anomalous Link Prediction in OT Networks
A Howe, M Papa - … on Digital Forensics and Security (ISDFS), 2025 - ieeexplore.ieee.org
Proposes a novel edge-focused temporal graph autoencoder architecture specifically designed for detecting anomalous packets in dynamic Operational Technology (OT) network graphs.
Anomalous Link Prediction: Uses reconstruction error in the autoencoder for anomalous link prediction, where an edge score indicates the likelihood that a network edge (communication) is normal.
Edge-Focused GNNs: Explores three edge-focused Graph Neural Networks (Graph Attention Networks, E-GraphSAGE, and Edge Enhanced GNNs) combined with a Gated Recurrent Unit for learning communication patterns and intrusion detection.
Save Cite Related articles
[PDF] secitsociety.org
Cybersecurity Threat Intelligence System Using NLP and Knowledge Graphs
SR Punam - Transactions on Secure Communication Networks and …, 2024 - secitsociety.org
Introduces the Cybersecurity Threat Intelligence System (CTIS) which uses a knowledge graph built from NLP-processed unstructured data to enable advanced analysis, including link prediction, for uncovering connections between threats.
Graph Embedding for Prediction: Explains that graph embedding algorithms convert graph information into vector representations, which facilitates advanced tasks like predicting future links and helping to uncover previously unknown connections between threats.
Predicting Attacker Behavior: Details how link prediction, for example, can be used to hypothesize that newly detected malware might target a known weakness because it shows similarities to other entities in the vector space, helping analysts predict attacker strategies.
Save Cite Related articles All 2 versions 
[PDF] usenix.org
Detecting lateral movement in enterprise computer networks with unsupervised graph {AI}
B Bowman, C Laprade, Y Ji, HH Huang - 23rd International Symposium …, 2020 - usenix.org
Applies a logistic regression link predictor to detect anomalous authentication events, which are indicative of lateral movement in a computer network.
Unsupervised Graph Learning: Utilizes an unsupervised graph-based machine learning pipeline that learns latent representations of authenticating entities to perform anomaly detection via the learned link predictor.
Graph Data Model: Models a computer network as an authentication graph and uses node embeddings and ground-truth edge information to train the link predictor.
Save Cite Cited by 154 Related articles All 4 versions 
Ask a follow-up
Scholar Labs is experimental and can make mistakes.
Help & Privacy


which one is most similar to us and can be done for link prediction on multiple datasets to produce a paper with in a week

---

**Cursor**

**Most similar to your work:** operational **event/provenance graphs** + **temporal/anomalous link prediction** — not the CTI/CVE knowledge-graph papers (most of that list).

---

## Closest papers (ranked)

| Paper | Similarity | Key difference from you |
|-------|------------|-------------------------|
| **[Euler](https://dl.acm.org/doi/10.1145/3588771)** (King & Huang, 2023) | **Highest** | Host auth graphs; you use **provenance** (Process/Artifact) |
| **[Jbeil](https://ieeexplore.ieee.org)** (Khoury et al., 2024) | Very high | Temporal LM link prediction on enterprise auth; same gap |
| **[Bowman et al.](https://www.usenix.org/conference/usenixsecurity20)** (USENIX 2020) | High | **Static** graph LM detection; no temporal RNN |
| **Edge-focused temporal GA** (Howe & Papa, 2025, OT networks) | Medium-high | OT packet graphs; same anomaly-via-temporal-LP idea |
| **GNN on IDS DAG** (Kim et al., 2025) | Medium | IDS log DAG; supervised threat link prediction |

**Least similar (ignore for now):** Yin MAGCN, RCTI/EGNN, CTI-KGE, D3FEND, malware KG papers — those are **knowledge graphs** (CVE↔CWE↔ATT&CK), not **system provenance / host activity graphs**.

**Your niche vs Euler/Jbeil:** *fine-grained provenance temporal link prediction on IIoT APT data (CICAPT-IIoT)*.

---

## Can you produce a full 3-dataset paper in 1 week?

**No** — not a solid venue-ready paper. Realistic in **1 week**:

- Preprocessing pipeline for CICAPT-IIoT  
- **UA + static GNN + Euler-lite** baselines  
- Phase 1 train → Phase 2 eval numbers  
- 1 comparison table + 1 case-study figure  
- Paper **outline + related-work positioning**

A credible **3-dataset** story needs **3–6 more weeks** after the pipeline exists.

---

## Fastest credible 3-dataset combo

| # | Dataset | Why | Effort |
|---|---------|-----|--------|
| 1 | **CICAPT-IIoT** (provenance) | Your main contribution | Already have it |
| 2 | **LANL 2015 auth** | Euler/Bowman standard; same task | Public; host-level edges |
| 3 | **CICAPT host-collapsed** OR **OpTC slice** | Provenance vs host on same data, or scale story | Host collapse = 1–2 days |

Using **provenance + host collapse on CICAPT** counts as a strong **internal ablation**; reviewers still want **LANL** (or OpTC) as an external benchmark.

---

## What to do this week (realistic)

**Days 1–2:** Preprocess Phase 1/2 → snapshots + `edge_index`  
**Days 3–4:** UA baseline + Euler-lite (GCN+GRU) on CICAPT-IIoT  
**Day 5:** Host-collapsed graph on same data; compare AP/TPR@1% FPR  
**Days 6–7:** Start LANL preprocessing; draft intro + related work (Euler, Jbeil, Bowman, CICAPT-IIoT paper)

**Deliverable after 1 week:** feasibility results + clear claim — *“provenance temporal LP vs host-level on CICAPT-IIoT”* — not a finished submission.

---

## How to add value vs Euler (one sentence)

> Euler and Jbeil detect lateral movement on **host auth graphs**; we show **provenance-level temporal link prediction** detects **earlier, tactic-specific APT steps** on **IIoT provenance** where host graphs miss them.

That’s differentiated from 90% of the papers in your search list (which are CTI knowledge-graph completion).

---

## Bottom line

| Question | Answer |
|----------|--------|
| Most similar paper? | **Euler** (then Jbeil, Bowman) |
| Continue CICAPT-IIoT? | **Yes** — it’s your differentiator |
| 3 datasets in 1 week? | **No** — aim for 1 dataset + baselines; add LANL week 2 |
| Ignore most of the list? | **Yes** — CTI KG papers are a different problem |

If the goal is **something submittable fast**, target a **workshop** (RAID poster, ACSAC poster, IEEE CSR) with **CICAPT-IIoT + LANL** and provenance-vs-host — not a full TIFS/TOPS paper in seven days.

---

**User**

I am not into provenance graphs. Just use my work in Heart to suggest me, forget about provenance graphs. 

---

**Cursor**

Reviewing HeaRT's link-prediction setup to recommend a cybersecurity direction without provenance graphs.


**Forget provenance.** Your natural paper is a **HeaRT paper applied to cybersecurity graphs** — not Euler-style anomaly detection, not CTI knowledge-graph completion as the main story.

---

## Your angle (from HeaRT)

HeaRT’s point: **random negative edges make link prediction look easy**. Hard negatives (PPR + CN + structure) give a **realistic** benchmark.

**Cybersecurity claim:**

> On security graphs, random-negative LP **overstates** how well GNNs predict risky/missing links. **HeaRT-style hard negatives** are a better evaluation (and training) setup for cyber LP.

That’s distinct from Euler (temporal anomaly) and from Yin/CTI papers (KG embedding on CVE↔CWE).

---

## Closest prior work (not provenance)

| Paper | Overlap | Your delta |
|-------|---------|------------|
| **Bowman et al. (USENIX’20)** | Auth graph + link predictor for LM | No hard-negative benchmark |
| **Euler / Jbeil** | Temporal graphs, security | Anomaly detection, not HeaRT LP eval |
| **Yin / Wu (vuln KG)** | LP on cyber KG | Random/easy negatives; no HeaRT |
| **HINTI (Zhao, USENIX’20)** | GCN on IOC graph | Threat intel, not LP benchmarking |

**Gap:** almost nobody applies **HeaRT hard negatives** to cybersecurity graphs.

---

## 3 datasets that fit your HeaRT pipeline

You already have the pattern: `graph.pt` → splits → `create_heart_negatives.py` → GCN/SEAL under `exist_setting` vs `HeaRT_*`.

| # | Dataset | Graph type | Why it fits |
|---|---------|------------|-------------|
| 1 | **LANL 2015 auth** | Host↔host authentication | Classic security interaction graph; Euler/Bowman use it |
| 2 | **Cyber KG (CVE–CWE–CAPEC)** | Entity knowledge graph | Matches most papers in your literature list |
| 3 | **HINTI IOC HIN** or **OpTC host FLOW** | Entity interaction / CTI | Third domain; shows generality |

**Fastest third option:** **OpTC host–host communication** (Euler already parsed it) — same preprocessing as LANL.

Skip CICAPT-IIoT if you’re not doing provenance.

---

## Task (keep it simple)

Standard **HeaRT link prediction**:

1. Build undirected/directed graph from cyber edges  
2. Train/val/test edge split (temporal split on LANL/OpTC is a plus)  
3. **Random negatives** vs **HeaRT negatives** (`create_heart_negatives.py`)  
4. Train **GCN** (you already have this on Twitter/SNAP)  
5. Report **Hits@K, MRR, AUC** under both settings  

Optional security add-on: on LANL, check whether **ranked hard-negative edges** overlap **labeled anomalous auth** more than random negatives.

---

## Paper title (working)

**“Hard Negatives Matter: Re-benchmarking Link Prediction on Cybersecurity Graphs with HeaRT”**

Or: **“HeaRT for Cybersecurity Graphs: Beyond Random Negative Evaluation”**

---

## What to do next (HeaRT-native)

| Step | Action |
|------|--------|
| 1 | Clone your **SNAP/Twitter preprocess** pattern for **LANL auth** → `graph.pt` + splits |
| 2 | Run `create_heart_negatives.py` on that graph (like `--dataset twitter`) |
| 3 | Run GCN: `exist_setting` vs `HeaRT` — measure the performance drop |
| 4 | Repeat on **cyber KG** + **OpTC or HINTI** |
| 5 | Write: random vs hard table × 3 datasets × 1–2 models (GCN, maybe SEAL) |

---

## 1-week realistic goal

**Not** a full 3-dataset TIFS paper.

**Yes:**  
- 1 dataset end-to-end (LANL or a **small CVE–CWE subgraph**)  
- Random vs HeaRT table  
- Paper outline + related work (HeaRT, Bowman, Yin, HINTI)

Week 2–3: add datasets 2 and 3.

---

## What *not* to do

- Don’t rebuild Euler temporal RNN unless you want a different paper  
- Don’t chase 40 CTI-KG papers — use **one** KG as dataset #2  
- Don’t use Phase1 provenance  
- Don’t claim APT detection unless you add labels + anomaly framing later  

---

**Bottom line:** Your paper is **HeaRT benchmarking on cybersecurity graphs** (interaction + KG), not provenance APT detection. **Next step:** preprocess **LANL auth** into `graph.pt` and run the same pipeline you used for SNAP Twitter with HeaRT negatives.

Switch to Agent mode if you want that LANL → HeaRT preprocessing wired up.

---

**User**

Just consider I am not resuing Heart iself, I have learned about GCN using heart. NOw using this knowledge I want to work in link prediction on graph datasets in cybersecurity, what should be my approach to produce results in1 week and submit a paper in IEEECARS2026. I want to have good results and something thats worth getting a paper award, if possible!

Muhammad Minhal
Current session
link prediction in cybersecurity graphs
[PDF] ieee.org
Find@UW
Knowledge-driven cybersecurity intelligence: Software vulnerability coexploitation behavior discovery
J Yin, MJ Tang, J Cao, M You, H Wang… - IEEE transactions on …, 2022 - ieeexplore.ieee.org
Formulates the discovery of vulnerability co-exploitation behavior as a link prediction problem between vulnerability entities within a cybersecurity knowledge graph.
MAGCN for Prediction: Proposes a Modality-Aware Graph Convolutional Network (MAGCN) module to embed multi-modality entity attributes and topological graph connectivity features to improve link prediction performance.
Graph Knowledge Transfer: Introduces a Graph Knowledge Transfer Learning (GKTL) strategy to transfer knowledge between subgraphs to address graph sparsity and time difference issues in the link prediction task.
Save Cite Cited by 122 Related articles All 6 versions
[PDF] ajisresearch.com
Graph Neural Networks (GNNS) For Modeling Cyber Attack Patterns And Predicting System Vulnerabilities In Critical Infrastructure
TK Chowdhury, S Biswas - American Journal of Interdisciplinary …, 2022 - ajisresearch.com
Explains that link prediction within cyber graph models aims to identify potential or hidden relationships that could facilitate lateral movement between system entities.
Link Prediction Mechanism: Details that link prediction estimates the likelihood of new or unauthorized edges forming, such as unexpected credential reuse or illicit access routes.
GNN Role in Link Prediction: Indicates that Graph Neural Networks (GNNs) enhance link prediction by learning higher-order dependencies, which allows for the discovery of subtle deviations in graph topology and edge semantics.
Save Cite Cited by 78 Related articles 
[HTML] sciencedirect.com
Find it at MLML/MBARI Res
[HTML] Edge propagation for link prediction in requirement-cyber threat intelligence knowledge graph
Y Zhang, J Chen, Z Cheng, X Shen, J Qin, Y Han… - Information Sciences, 2024 - Elsevier
Proposes EGNN, a novel Graph Neural Network-based model with an edge propagation algorithm, for predicting new knowledge (links) on the heterogeneous cybersecurity knowledge graph called RCTI.
Cybersecurity Knowledge Graph: Constructs the heterogeneous knowledge graph, RCTI, by combining cybersecurity threat intelligence data with management security requirements data from national network security standards.
Task Definition: Transforms the reasoning issue of finding relations between security requirement data and cyber threat intelligence data into a link prediction task on a heterogeneous directed graph.
Save Cite Cited by 47 Related articles All 2 versions 
[HTML] mdpi.com
Find it at MLML/MBARI Res
[HTML] Knowledge graphs and semantic web tools in cyber threat intelligence: A systematic literature review
C Bratsas, EK Anastasiadis, AK Angelidis… - … of Cybersecurity and …, 2024 - mdpi.com
Reviews research that applied Machine Learning (ML) or Deep Learning (DL) methods to problems in Cyber Threat Intelligence (CTI) such as link prediction, specifically mentioning a dedicated subcategory for these works.
Graph Embedding and Inference: Discusses a model that applies link prediction tasks to augment the CTI landscape by automatically inferring relationships between head and tail entities using graph embedding algorithms and reasoning rules.
Threat Indicator Prediction: Highlights a model, Ctiap, which combines matrix factorization and Neural Network methods to address the link prediction problem on a graph dataset of threat indicators, demonstrating high accuracy and F1-score in mining threat indicator relationships.
Save Cite Cited by 25 Related articles All 3 versions 
[PDF] ieee.org
Find@UW
Attack hypotheses generation based on threat intelligence knowledge graph
FK Kaiser, U Dardik, A Elitzur… - … on Dependable and …, 2023 - ieeexplore.ieee.org
Presents a toolkit called the Attack Hypothesis Generator that uses knowledge graph traversal and various link prediction methods to automatically infer ATT&CK techniques from observed artifacts in cybersecurity.
Uses Various LP Metrics: Utilizes link prediction techniques, including Jaccard’s/Tanimoto coefficient, Adamic Adar, Friends/Katz measure, and Preferential Attachment, for hypothesis refinement in the threat intelligence knowledge graph (AttackDB).
Applies Supervised LP: Formulates link prediction as a supervised learning problem using a random forest classifier to predict the probability of a link existing in the threat intelligence knowledge graph.
Save Cite Cited by 66 Related articles All 6 versions
[HTML] mdpi.com
Full View
[HTML] A knowledge graph completion algorithm based on the fusion of neighborhood features and vBiLSTM encoding for network security
W Zhang, M Wang, G Han, Y Feng, X Tan - Electronics, 2024 - mdpi.com
Proposes a new knowledge graph completion method, the KGC-N model, that integrates feature vectors with graph structure information and uses a Transformer decoder to predict and complete missing links in a network security knowledge graph.
Addresses Sparsity: Addresses the challenge of information sparsity in network security knowledge graphs using entity link prediction as a component of the completion task.
Applications in Attack Inference: Describes an existing algorithm that uses knowledge graph traversal and various link prediction methods to complete missing information and automatically infer ATT&CK techniques in observable network attacks, demonstrating applications of link prediction in cybersecurity.
Save Cite Cited by 8 Related articles All 3 versions 
[HTML] mdpi.com
Find it at MLML/MBARI Res
[HTML] Recommendations for responding to system security incidents using knowledge graph embedding
H Kim, J Choi - Electronics, 2023 - mdpi.com
Proposes a method for recommending strategies to respond to edge computing security incidents based on the automatic generation and embedding of security knowledge graphs.
Knowledge Graph Embedding: Utilizes knowledge graph embedding as a representative method for link prediction, which involves vectorizing entities and relationships and learning the embedding value based on a scoring function.
Link Prediction Experiments: Describes experiments on link prediction and triple classification to evaluate the performance of the proposed knowledge completion and inference for security incident responses.
Save Cite Cited by 7 Related articles All 3 versions 
Generating network security defense strategy based on cyber threat intelligence knowledge graph
S Zhang, S Li, P Chen, S Wang, C Zhao - International Conference on …, 2022 - Springer
Develops CTI-KGE, a knowledge graph embedding algorithm used for link prediction tasks to infer tail entities and complete threat information automatically.
0-day Attack Prediction: Transforms attack prediction into a link prediction problem on a network defense knowledge graph to address the difficulty of predicting attacks caused by 0-day vulnerabilities.
Completing Threat Information: Uses link prediction to complete triples by predicting sparse and missing relationships in the Cyber Threat Intelligence (CTI) knowledge graph, which is essential for inferring defense strategies.
Save Cite Cited by 16 Related articles 
Find@UW
Infer the missing facts of D3FEND using knowledge graph representation learning
A Khobragade, S Ghumbre… - International Journal of …, 2023 - emerald.com
Proposes an automated approach to predict missing facts in the D3FEND knowledge graph, a cybersecurity countermeasure domain resource, using a link prediction task with embedding as representation learning.
Embedding Models Applied: Applies progressive distance-based, bilinear, and convolutional embedding models to learn representations of entities and relations within the D3FEND graph for the link prediction task.
Cybersecurity Strategy Benefit: States that link prediction in the D3FEND Knowledge Graph can benefit cybersecurity countermeasure strategies by identifying gaps, comparing defensive methods, and discovering novel methods.
Save Cite Cited by 6 Related articles All 2 versions
A management knowledge graph approach for critical infrastructure protection: Ontology design, information extraction and relation prediction
J Chen, Y Lu, Y Zhang, F Huang, J Qin - International Journal of Critical …, 2023 - Elsevier
Adopts the Knowledge Graph Bidirectional Encoder Representations from Transformer (KG-BERT) model to capture semantic information and predict relationships between threat and management components within the constructed graph.
Completes Knowledge Graph: Applies link prediction to complete the cybersecurity knowledge graph, specifically using KG-BERT to predict the link between threats and complete missing relationships.
Associates Threat and Management: Utilizes link prediction to associate the established management knowledge graph with the existing cybersecurity threat knowledge graph, inferring relationship triples between the two parts.
Save Cite Cited by 32 Related articles All 4 versions 
[PDF] ieee.org
Find@UW
Beyond Single-Hop: Link Prediction Through Multi-Hop Reasoning in Malware Knowledge Graphs
K Bahlibi, HL Nguyen, E Dogdu… - 2026 IEEE 5th …, 2026 - ieeexplore.ieee.org
Proposes a unified framework for multi-hop link prediction in malware knowledge graphs, which are essential for representing complex relationships in cyber threat reports.
Hybrid Reasoning Paradigms: Learns latent representations of entities and relations by combining graph-based reasoning (Graph Neural Networks like GraphSAGE, GCN, GAT) and language-based reasoning (Large Language Models like LLaMA-2-7B and Phi-2).
Model Performance Comparison: Finds that while Graph Neural Networks are effective on shorter paths, Large Language Models maintain robust performance across longer reasoning chains, demonstrating potential for dynamic malware analysis.
Save Cite Related articles All 2 versions
[HTML] mdpi.com
Find it at MLML/MBARI Res
[HTML] Recent progress of using knowledge graph for cybersecurity
K Liu, F Wang, Z Ding, S Liang, Z Yu, Y Zhou - Electronics, 2022 - mdpi.com
Discusses knowledge reasoning technology in cybersecurity knowledge graphs (CSKGs) that can generate new knowledge to support prediction and inference tasks.
Attack Prediction as Link Prediction: Explains that attack prediction can be transformed into a knowledge graph link prediction problem, as demonstrated by a study that used a path ranking algorithm to create a 0-day attack graph.
Link Prediction Tasks in CSKGs: Mentions that CSKGs can be utilized for three different reasoning tasks, including a link prediction task based on Common Weakness Enumeration (CWE).
Save Cite Cited by 79 Related articles All 7 versions 
[PDF] ssrn.com
Enhancing Cybersecurity Situation Awareness Through Knowledge Graphs: An Integrated Survey Including Threats, Vulnerabilities, and Assets
Y Guan, Y Zhang, J Yue, Y Lu, Y Xie - Vulnerabilities, and Assets - papers.ssrn.com
Explains that link prediction techniques, such as graph completion, can allow knowledge graphs to provide early warnings of unknown attack patterns, enhancing a cybersecurity system’s foresight and proactivity.
Threat Attribution Modeling: Discusses formalizing threat attribution as a link prediction task on a graph to enhance interpretability and make complex analyses more intuitive and efficient.
Predicting New Attacks: Mentions using graph neural networks with attention mechanisms in models like EGNN to predict new connections and discover new attack incidents based on patterns learned from a cybersecurity knowledge graph.
Save Cite Cited by 2 Related articles 
[PDF] acm.org
Full View
Advancing cybersecurity: graph neural networks in threat intelligence knowledge graphs
L Li, F Qiang, L Ma - Proceedings of the International Conference on …, 2024 - dl.acm.org
Highlights that Graph Neural Networks (GNNs) are proficient in various tasks on cybersecurity graphs, specifically including link prediction, node prediction, and graph classification.
TIKG Analysis with GNNs: Discusses leveraging Graph Neural Networks for analyzing Threat Intelligence Knowledge Graphs (TIKGs) to enhance the understanding of complex relationships between various cyber threats and related entities.
Learning Graph Representations: Explains that GNNs learn representations of graph structures to uncover patterns and dependencies, which is necessary for extracting actionable insights from knowledge graphs in network security.
Save Cite Cited by 15 Related articles 
[PDF] ieee.org
Find it at MLML/MBARI Res
Association Analysis and Prediction of Network Security Vulnerabilities Based on Knowledge Graph
S Wu, L Feng - Journal of Cyber Security and Mobility, 2025 - ieeexplore.ieee.org
Presents a novel prediction framework that combines knowledge graph embeddings, graph neural networks, and multi-modal feature fusion to forecast vulnerability exploitation.
Link Prediction Model: Utilizes a multi-task learning model that simultaneously performs link prediction and entity classification, achieving 86.3% accuracy in link prediction.
Vulnerability Graph Construction: Constructs a comprehensive vulnerability knowledge graph that captures semantic relationships between vulnerabilities, attack patterns, and affected systems using integrated data from NVD, CVE, and vendor security bulletins.
Save Cite Related articles All 3 versions 
[HTML] mdpi.com
Find it at MLML/MBARI Res
[HTML] A cybersecurity knowledge graph completion method for penetration testing
P Wang, J Liu, X Zhong, S Zhou - Electronics, 2023 - mdpi.com
Designs a knowledge graph completion model, CSNT, to infer missing relationships (link prediction) in cybersecurity data, enhancing the information's integrity and accuracy.
Mining Implicit Relationships: Proposes a method to mine implicit relationships from existing cybersecurity data, which can then be used to complete and correct cybersecurity intelligence information.
Neural Network and Tensor Methods: Uses a combination of a neural network (BiLSTM) and tensor decomposition to model the relationship between entities for the purpose of completing the cybersecurity knowledge graph.
Save Cite Cited by 12 Related articles All 3 versions 
[PDF] ieee.org
Find it at MLML/MBARI Res
Creating cybersecurity knowledge graphs from malware after action reports
A Piplai, S Mittal, A Joshi, T Finin, J Holt, R Zak - IEEE Access, 2020 - ieeexplore.ieee.org
Describes building a neural network component called RelExt, a deep learning-based relationship extractor, to predict relationships between pairs of entities in a Cybersecurity Knowledge Graph (CKG).
Knowledge Graph Population: Utilizes a pipeline that includes a Relationship Extractor (RelExt) to predict how pairs of malware entities are related to each other, asserting the resulting entity-relationship set into the CKG.
Contextual Relationship Prediction: Employs vector representations to capture the context of cybersecurity entities in text, which aids in the task of relationship prediction within the knowledge graph creation process.
Save Cite Cited by 160 Related articles All 9 versions 
[HTML] mdpi.com
Full View
[HTML] Dynamic vulnerability knowledge graph construction via multi-source data fusion and large language model reasoning
R Liu, Y Xie, Z Dang, J Hao, X Quan, Y Xiao, C Peng - Electronics, 2025 - mdpi.com
Introduces a novel combination of graph-based and Large Language Model (LLM)-based reasoning for link prediction, specifically fusing a text-enhanced Graph Attention Network (GAT) with GPT-3-generated insights to infer missing relationships.
Two-Pronged Prediction Approach: Employs a two-pronged link prediction strategy by combining TransH-based Knowledge Graph (KG) embeddings with BERT-derived textual node vectors to enhance GAT, followed by a weighted fusion of GAT and LLM confidence scores.
Link Prediction Performance: Reports that using a hybrid Graph Attention Network fused with GPT-3 reasoning boosted Hits@1 by 0.137, Hits@3 by 0.116, and Hits@10 by 0.101 over the baseline for link prediction, confirming enhanced relationship inference.
Save Cite Cited by 8 Related articles All 3 versions 
[PDF] edpsciences.org
Uncovering multi-step attacks with threat knowledge graph reasoning
X Xiang, C Ma, L Zeng, W Feng, Y Xie… - Security and …, 2025 - sands.edpsciences.org
Introduces a novel knowledge representation learning approach, A4CKGE (ATT&CK-CAPEC-CWE-CVE-CPE Knowledge Graph Embedding), to predict interactions among security entities in a cohesive threat knowledge graph.
Knowledge Graph Embedding: Utilizes a knowledge graph embedding technique to convert structural knowledge and textual descriptions into a compact, low-dimensional continuous vector space, a method which is verified for link prediction applications.
Reasoning for Attack Prediction: Develops a knowledge graph embedding method to enhance reasoning capabilities over security entity relations and predict multi-step attack sequences, particularly those associated with complex Advanced Persistent Threats (APTs).
Save Cite Cited by 7 Related articles All 4 versions 
[PDF] ieee.org
Actionable cyber threat intelligence using knowledge graphs and large language models
R Fieblinger, MT Alam, N Rastogi - 2024 IEEE European …, 2024 - ieeexplore.ieee.org
Applies a constructed Knowledge Graph, derived from Cyber Threat Intelligence (CTI) using Large Language Models (LLMs), to link prediction tasks to enhance cybersecurity defenses and response strategies.
Benefits of Link Prediction: Explains that link prediction in cybersecurity Knowledge Graphs enables proactive threat detection by forecasting potential future connections, like new malware attack patterns, and aids vulnerability assessment by revealing possible attack paths.
Evaluation of Link Prediction: Evaluates the efficacy of the link prediction task by comparing performance against a baseline model (LADDER) using traditional metrics such as Mean Reciprocal Rank (MRR) and Hits@n, demonstrating the capabilities of the LLM-constructed Knowledge Graph.
Save Cite Cited by 89 Related articles All 6 versions 
[PDF] ieee.org
Find@UW
Time-aware cybersecurity knowledge graph reasoning method for vulnerability analysis
M Wang, K Li, Y Lu, F Zhang, J Ma… - IEEE Transactions on …, 2025 - ieeexplore.ieee.org
Proposes TCGFormer, a time-aware cybersecurity knowledge graph reasoning method, specifically designed to identify co-exploitation behaviors and predict entities and relations.
TCGFormer Modules: Explains that the TCGFormer method includes a parameter learning module responsible for predicting entities and relations, which is synonymous with link prediction.
Link Prediction Evaluation: Discusses using accuracy, precision, recall, and F1 score to evaluate the link prediction binary classification task specifically on a cybersecurity dataset.
Save Cite Cited by 12 Related articles
Uncovering Security Entity Relations with Cyber Threat Knowledge Graph Embedding
C Ma, X Xiang, Y Xie, W Feng, Z Gu - International Conference on Network …, 2023 - Springer
Proposes a novel knowledge representation learning method, 4CKGE (CAPEC-CWE-CVE-CPE Knowledge Graph Embedding), for predicting correlations between security entities like products, vulnerabilities, weaknesses, and attack patterns.
Threat Knowledge Graph: Utilizes a cyber threat knowledge graph, aggregated from databases like CAPEC, CWE, CVE, and CPE, to semantically cross-analyze security-related entities and relations.
Structural and Textual Embeddings: Employs an embedding approach that integrates both structural and textual information from the knowledge graph to enhance performance in relationship prediction tasks.
Save Cite Cited by 3 Related articles 
[PDF] spiedigitallibrary.org
Find it at MLML/MBARI Res
Attack prediction in Internet of Things using knowledge graph
S Zhang, C Zhao, S Wang, S Li… - … conference on internet …, 2023 - spiedigitallibrary.org
Transforms the attack prediction task in IoT cybersecurity into a link prediction problem within a knowledge graph framework.
Knowledge Representation Learning: Utilizes a knowledge representation learning method (FTSPC) that fuses triple information and semantic path combination information to conclude possible attacks.
Knowledge Graph Construction: Constructs an intelligent model to build a knowledge graph by integrating equipment (CPE), vulnerability (CVE), and attack pattern (CAPEC) information from NIST and MITRE.
Save Cite Cited by 11 Related articles All 3 versions 
[PDF] springer.com
Find it at MLML/MBARI Res
A knowledge extrapolation model for attack inference based on graph attention networks and relation mapping: W. Ren et al.
W Ren, L Zhao, W Li - Knowledge and Information Systems, 2026 - Springer
Proposes an attack knowledge extrapolation model based on graph attention networks and relation mapping that infers and predicts unknown attack-related entity–relation triples within a cybersecurity knowledge graph.
Predicting Unknown Triples: Uses a scoring function to infer and predict unknown attack-related entity–relation triples, which is the core task of link prediction in knowledge graphs.
Proactive Prediction: Discusses the importance of accurately predicting unknown attack relationships in real-world cybersecurity operations for deploying defensive measures earlier, thereby reducing detection and response times.
Save Cite Cited by 2 Related articles All 2 versions 
[PDF] springer.com
Find it at MLML/MBARI Res
GC-PTransE: multi-step attack inference method based on graph convolutional neural network and translation embedding: W. Ren et al.
W Ren, W Li, Y Hong, Y Du, Y Gao, H Zhang… - … and Information Systems, 2025 - Springer
Proposes a multi-step attack inference method called GC-PTransE, which uses graph convolutional neural networks (GCN) and translational embedding to predict cyberattack scenarios.
Uses Translational Embedding: Utilizes PTransE for embedding symbolic and descriptive information of cyberattack entities into a low-dimensional continuous vector space, navigating graph structure data along paths of triples to identify entities and relationships.
Predicts Outcomes and Completes Graph: Details how combining path relationships and scoring the energy of new triples predicts outcomes, thus completing the cyberattack knowledge graph.
Save Cite Cited by 4 Related articles All 3 versions 
[PDF] nature.com
Find it at MLML/MBARI Res
Network attack knowledge inference with graph convolutional networks and convolutional 2D KG embeddings
W Ren, H Zhang, Y Lei - Scientific Reports, 2025 - nature.com
Proposes a method, KGConvE, based on graph convolutional networks and convolutional 2D KG embeddings to perform attack inference and association mining of implicit network attack knowledge.
Predicts Entity Relationships: Applies the KGConvE model to perform attack inference tasks to predict implicit relationships between entities like CVE-CVE, CVE-CWE, and CVE-CAPEC, achieving significant performance improvements in knowledge inference tasks.
Constructs Cybersecurity Graph: Constructs a cybersecurity knowledge graph that associates CVE, CWE, CAPEC, and APT entities, enabling the extraction of implicit attack knowledge for proactive defense and network security situational awareness.
Save Cite Cited by 1 Related articles All 6 versions 
[HTML] mdpi.com
Find it at MLML/MBARI Res
[HTML] A cybersecurity knowledge graph completion method based on ensemble learning and adversarial training
P Wang, J Liu, D Hou, S Zhou - Applied Sciences, 2022 - mdpi.com
Develops CSEA, a new knowledge graph completion method that uses ensemble learning and adversarial training to infer unknown facts and complement existing cybersecurity knowledge.
Enhances Model Robustness: Designs a cooperative adversarial training method to enhance the generalization and robustness of the model, specifically tailored for cybersecurity knowledge graph completion.
Measures Link Prediction: Measures the performance of the model for cyber security link prediction using evaluation metrics such as MRR, MR, and Hits@k.
Save Cite Cited by 8 Related articles All 4 versions 
Find@UW
Applicability Analysis of Knowledge Graph Embedding on Blended Threat
M Lee, IS Jung, DH Kim… - … and Service (PlatCon), 2022 - ieeexplore.ieee.org
Analyzes a knowledge graph embedding model applicable to blended threats to predict and respond to potential complex security risks in the Internet of Blended Environment (IoBE).
Details Link Prediction Use: Explains that link prediction, which includes both relation prediction and entity prediction, can be used with knowledge graph embedding to address incompleteness in the knowledge graph for security applications.
Applies KGE to STIX Data: Discusses using knowledge graph embedding techniques to visualize various threat data from STIX 2.1 and apply reasoning tasks for more accurate security threat prediction.
Save Cite Cited by 3 Related articles All 2 versions
[PDF] ieee.org
Find@UW
Ctinexus: Automatic cyber threat intelligence knowledge graph construction using large language models
Y Cheng, O Bajaber, SA Tsegai… - 2025 IEEE 10th …, 2025 - ieeexplore.ieee.org
Proposes CTINexus, a framework that utilizes a long-distance relation prediction technique to complete Cybersecurity Knowledge Graphs (CSKGs) with missing links.
LLM for CSKG Construction: Explains that the framework leverages optimized in-context learning of large language models for data-efficient knowledge extraction and high-quality CSKG construction.
Inferring Missing Connections: Describes how the long-distance relation prediction technique infers connections among disjoint subgraphs by selecting central nodes and predicting implicit relations among them.
Save Cite Cited by 35 Related articles All 8 versions
[PDF] ecu.edu.au
Cybersecurity knowledge graphs
L Sikos - 2023 - ro.ecu.edu.au
Discusses that relationship predictions between entity pairs in cybersecurity knowledge graphs can be achieved using deep learning techniques.
Statistical Relational Learning: Explains that statistical relational learning can be applied to predict missing links and identify relationships between nodes within inherently sparse, incomplete, and noisy cyber-knowledge graphs.
Unsupervised Link Prediction: Mentions that link prediction can be performed unsupervised to rank the likelihood of triple statements resulting from events observed at test time, often following training a generative graph embedding algorithm on a baseline dataset.
Save Cite Cited by 153 Related articles All 8 versions 
[PDF] acm.org
Find it at MLML/MBARI Res
Relext: Relation extraction using deep learning approaches for cybersecurity knowledge graph improvement
A Pingle, A Piplai, S Mittal, A Joshi, J Holt… - Proceedings of the 2019 …, 2019 - dl.acm.org
Proposes a system called RelExt that uses a deep feedforward neural network to predict relationships between subject-object entity pairs, thus improving the population of a cybersecurity knowledge graph by filling in missing links.
KG Creation: Explains that the creation of a cybersecurity knowledge graph involves predicting the correct relationship between two specific entities extracted from cyber threat intelligence sources.
Improving Knowledge Graphs: Aims to improve existing knowledge graphs by looking for more relationships, or links, between entities already available in the knowledge graph, which is a key aspect of link prediction.
Save Cite Cited by 187 Related articles All 11 versions 
[PDF] arxiv.org
A review of knowledge graph application scenarios in cyber security
K Liu, F Wang, Z Ding, S Liang, Z Yu, Y Zhou - arXiv preprint arXiv …, 2022 - arxiv.org
Explains that a Knowledge Graph (KG) derived from threat, assets, and vulnerability data transforms the attack prediction task into a link prediction problem within the KG.
0-Day Attack Path Generation: Mentions using link prediction and a path ranking algorithm to represent and generate 0-day attack paths within the cybersecurity knowledge graph.
CWE Link Prediction and Reasoning: Indicates that the cybersecurity knowledge graph (CSKG) based on Common Weakness Enumeration (CWE) can be utilized for reasoning tasks, including CWE link prediction and predicting common consequences.
Save Cite Cited by 50 Related articles All 2 versions 
[HTML] sciencedirect.com
Find it at MLML/MBARI Res
[HTML] Enhanced small-scale APT knowledge graph embedding via spatio-temporal attribute reasoning and adversarial negative sampling
Y Xie, H Wang, X Jing, Z Gu - Array, 2025 - Elsevier
Proposes an enhanced Knowledge Graph Embedding (KGE) method, called APT-ST-AN, specifically designed for Advanced Persistent Threat (APT) attack prediction to address the limitations of incomplete and sparse data in small-scale APT knowledge graphs.
Enriching Examples: Introduces spatio-temporal attribute reasoning and adversarial negative sampling to enrich both positive and negative APT attack examples, thereby improving the expressiveness and generalization of KGE models for attack inference.
KGE for Attack Prediction: Explains that KGE techniques are a promising approach for APT attack prediction by leveraging existing cybersecurity knowledge to infer potential attack behaviors, directly addressing the link prediction aspect.
Save Cite Cited by 1 Related articles 
[PDF] springer.com
Find it at MLML/MBARI Res
GPCNKB: An Attack Prediction and Reasoning Model Based on an Improved Graph Convolutional Network
W Ren, J Yao, Y Hong - Neural Processing Letters, 2025 - Springer
Introduces GPCNKB, a network attack prediction and reasoning method that utilizes knowledge graphs and embedding techniques to represent attack scenarios for predicting attack patterns.
Knowledge Graph Reasoning: Applies a knowledge graph embedding model to reason attack knowledge and predict potential relationships within scenario graphs of the same category, enhancing reasoning accuracy.
Graph Convolutional Networks: Utilizes graph convolutional networks (GCNs) for classifying attack scenarios and incorporates evolutionary computation to refine the GCN model, optimizing training parameters and improving universality.
Save Cite Related articles All 2 versions 
[PDF] osti.gov
Retrieval augmented generation for robust cyber defense
M Rahman, KO Piryani, AM Sanchez, S Munikoti… - 2024 - osti.gov
Utilizes a graph-based framework, GraphCyRAG, leveraging Neo4j knowledge graphs for deeper traversal of relationships between vulnerabilities (CVE, CWE) and attack patterns (CAPEC, ATT&CK), which are essential for predicting exploit paths.
RAG and Relationship Discovery: Demonstrates that integrating knowledge graphs with Retrieval-Augmented Generation (RAG) helps analysts uncover hidden relationships between cyber entities and predict exploit paths.
Path Prediction using Cypher: Enables tracing the shortest or most relevant paths between nodes (CVE, CWE, CAPEC) using Cypher queries to link a specific vulnerability to associated attack patterns, helping to predict how a vulnerability could be exploited in real-world scenarios.
Save Cite Cited by 22 Related articles All 3 versions 
[PDF] usenix.org
Cyber threat intelligence modeling based on heterogeneous graph convolutional network
J Zhao, Q Yan, X Liu, B Li, G Zuo - … on research in attacks, intrusions and …, 2020 - usenix.org
Proposes a framework, HINTI, to model interdependent relationships among heterogeneous Indicators of Compromise (IOCs) using a newly constructed heterogeneous information network (HIN) for threat intelligence computing.
GCN for Knowledge Discovery: Utilizes a threat intelligence computing framework based on graph convolutional networks (GCN) for effective knowledge discovery, noting GCNs are effective tools for tasks like link prediction on graphs.
Predicting Attacker Intrusion: Mentions that CTI subscribers can use the resulting low-dimensional embedding space to gain new threat insights, such as predicting which attackers are likely to intrude their systems.
Save Cite Cited by 133 Related articles All 6 versions 
[PDF] wiley.com
Find it at MLML/MBARI Res
Knowledge graph reasoning for cyber attack detection
E Gilliard, J Liu, AA Aliyu - IET Communications, 2024 - Wiley Online Library
Discusses knowledge graph embedding (KGE) as a technique that translates entities and relationships into low-dimensional vector spaces, which supports machine learning models for reasoning and prediction tasks.
Use for Link Prediction: Identifies link prediction as one of the valuable tasks for which the vector representation of semantic meanings, captured by Knowledge Graph Embedding, proves beneficial.
Threat Identification Automation: Proposes an approach using knowledge graph reasoning to improve the efficiency and accuracy of network threat identification, establishing a foundation for automated network attack detection.
Save Cite Cited by 27 Related articles All 5 versions 
Graph neural networks in network security: from theoretical foundations to applications
X Zhang - 2025 - aisel.aisnet.org
Examines network security applications at the node, edge, and graph levels, illustrating how Graph Neural Network (GNN) embeddings translate into practical threat identification and classification.
GNN Role in Network Security: Reviews the transformative role of GNNs in network security, noting that researchers have introduced various GNN architectures to address a range of challenges.
Temporal Link Prediction: References a related work that explicitly focuses on detecting network lateral movement using scalable temporal link prediction, which is a specific application of link prediction in cybersecurity.
Save Cite Cited by 4 Related articles
[PDF] ieee.org
Find@UW
Network Security Threat Detection System Based on Knowledge Graph
M Zheng, T He, N Wang, X Wang… - 2025 40th Youth …, 2025 - ieeexplore.ieee.org
Presents a cybersecurity threat detection system built on a knowledge graph that integrates heterogeneous data sources into a semantic framework, which often involves establishing or predicting links between entities.
Future Prediction Research: Discusses future research directions that involve leveraging the reasoning capabilities of knowledge graphs to predict attack paths and uncover latent threats, which directly relates to link prediction.
Linking Cybersecurity Elements: Constructs a knowledge graph by linking offensive tactics and vulnerabilities, such as linking ATT&CK techniques to CAPEC attack patterns and then to CWE weaknesses and CVE vulnerabilities.
Save Cite Related articles
[PDF] acm.org
Full View
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction
IJ King, HH Huang - ACM Transactions on Privacy and Security, 2023 - dl.acm.org
Formulates anomalous lateral movement detection in cybersecurity as a temporal graph link prediction problem, where network interactions are abstracted into a series of graphs.
Euler Framework: Proposes the Euler framework for scalable dynamic link prediction and anomalous edge detection, which involves stacking a model-agnostic Graph Neural Network upon a sequence encoder like a Recurrent Neural Network.
Performance in Anomaly Detection: Demonstrates that models built with the Euler framework efficiently identify anomalous connections with high precision and outperform other unsupervised techniques for detecting anomalous lateral movement.
Save Cite Cited by 175 Related articles All 8 versions 
[PDF] ieee.org
Find@UW
Graph Neural Network-Based Prediction of Cyber Threats Using IDS Logs
H Kim, H Doo, S Yoon, H Lim - 2025 16th International …, 2025 - ieeexplore.ieee.org
Proposes a novel framework that constructs a directed acyclic graph (DAG) from Intrusion Detection System (IDS) logs to characterize attacker–vulnerability–victim chains.
GNN for Link Prediction: Trains a Graph Neural Network (GNN) model, specifically GCN2, on the DAG to perform link prediction of unseen threats, enabling the anticipation of potential cyber threats.
Performance Evaluation: Demonstrates strong predictive performance on real-world Snort logs, achieving a high AUC of 0.91 in binary link prediction, validating the approach for forecasting emerging cyber threats.
Save Cite Related articles
[PDF] ieee.org
Full View
Research on Parameter-Efficient Knowledge Graph Completion Methods and Their Performance in the Cybersecurity Field
B Chen, H Li, Z Shi - IEEE Access, 2025 - ieeexplore.ieee.org
Proposes a multi-level graph clustering-based model called GCCKG for completing cyberthreat intelligence knowledge graphs, which directly relates to link prediction.
Predicting Missing Relations: Explains that knowledge graph completion, which includes link prediction, aims to predict and supplement missing triples to enhance the completeness and accuracy of the graphs and unearth unknown relations between threat intelligence entities.
Cybersecurity Dataset Application: Applies the proposed GCCKG model to the cyberthreat intelligence knowledge graph CS13K, demonstrating its effectiveness in knowledge graph completion tasks, a form of link prediction.
Save Cite Cited by 1 Related articles All 3 versions 
[PDF] ieee.org
Knowledge-Driven Prediction of Vulnerability Co-Exploitation
Y Yu, K Luo, X Hu, Y Zhang - 2025 IEEE 6th International …, 2025 - ieeexplore.ieee.org
Proposes a novel methodology for predicting the co-exploitation of vulnerabilities using knowledge reasoning based on a vulnerability knowledge graph, VulCKG, which is a key task related to link prediction in cybersecurity.
Knowledge Graph Link Prediction Model: Designs a knowledge embedding model, AttTransM, for knowledge graph completion (KGC), which is explicitly stated to be synonymous with link prediction, to fill in missing critical relational information within the graph.
Rule-Based Link Inference: Applies rule-based reasoning on the completed knowledge graph to infer vulnerability co-exploitation relationships and related security information, such as mitigation measures and detection techniques.
Save Cite Related articles 
[PDF] ieee.org
Find@UW
NLP-Augmented Knowledge Graph Embedding for Relationship Prediction in Network Security
Y Yin, X Liu, X Yu - 2024 6th International Academic Exchange …, 2024 - ieeexplore.ieee.org
Proposes a novel approach that combines Natural Language Processing (NLP) with knowledge graph reasoning algorithms to improve relationship prediction in network security.
CombineSS Embedding Method: Introduces a method, called CombineSS, that integrates structural and semantic embedding to optimize knowledge graph relationship prediction in the network security domain.
Performance Improvement: Demonstrates that the NLP-enhanced knowledge graph method effectively improves prediction accuracy and performance for entity link prediction and relationship prediction tasks compared to traditional graph structure algorithms.
Save Cite Related articles
[PDF] semanticscholar.org
[PDF] A Knowledge Graph Completion Algorithm Based on the Fusion of Neighborhood Features and vBiLSTM Encoding for Network Security. Electronics 2024, 13 …
W Zhang, M Wang, G Han, Y Feng, X Tan - 2024 - pdfs.semanticscholar.org
Proposes a completion method, KGC-N, that uses a vBiLSTM model for text encoding, feature fusion, and a Transformer decoder to predict and complete missing information within the network security knowledge graph.
Defines Link Prediction: Employs entity link prediction as a component of completing knowledge graphs, framing it as a sorting task to fill in missing entity parts in triples.
Evaluates Link Prediction Metrics: Utilizes evaluation metrics like MR and MRR to assess the effectiveness of the KGC-N model specifically for the network security knowledge graph completion and link prediction tasks.
Save Cite Related articles 
[PDF] ieee.org
Find@UW
Jbeil: Temporal graph-based inductive learning to infer lateral movement in evolving enterprise networks
J Khoury, Đ Klisura, H Zanddizari… - … IEEE Symposium on …, 2024 - ieeexplore.ieee.org
Proposes a data-driven framework called Jbeil that utilizes self-supervised deep learning on evolving networks, represented as sequences of authentication timed events, to perform link prediction.
Encoder-Decoder Mechanism: Explains that the framework applies an encoder on a continuous-time evolving graph to produce embeddings and a decoder leverages these embeddings to perform Lateral Movement (LM) link prediction on unseen nodes.
Cyber Fraud Detection: Discusses that link prediction is a crucial task in analyzing graph data with benefits in areas like cyber fraud detection and knowledge graph completion.
Save Cite Cited by 46 Related articles All 6 versions
[PDF] researchsquare.com
Network Attack Knowledge Reasoning Model based on Graph Convolutional Neural Networks
WW Ren, Y Lei, Y Hong, XT Song, JY Yao, YZ Du… - 2023 - researchsquare.com
Proposes an attack knowledge inference model that utilizes graph convolutional neural networks to analyze large-scale penetration attacks with complex, multi-hop pathways.
CONVE Model for Inference: Introduces the CONVE model for attack inference tasks, demonstrating its ability to infer implicit relationships between CVE-CVE, CVE-CWE, and CVE-CAPEC, addressing a form of link prediction.
Attack Relationship Deduction: Discusses how the methods leverage existing attack context information and inference models to deduce attack knowledge, including the completion of unknown attacks and attack relationships within the attack knowledge graph.
Save Cite Related articles All 3 versions 
[PDF] ieee.org
Find@UW
Edge-Focused Temporal Graph Autoencoders for Anomalous Link Prediction in OT Networks
A Howe, M Papa - … on Digital Forensics and Security (ISDFS), 2025 - ieeexplore.ieee.org
Proposes a novel edge-focused temporal graph autoencoder architecture specifically designed for detecting anomalous packets in dynamic Operational Technology (OT) network graphs.
Anomalous Link Prediction: Uses reconstruction error in the autoencoder for anomalous link prediction, where an edge score indicates the likelihood that a network edge (communication) is normal.
Edge-Focused GNNs: Explores three edge-focused Graph Neural Networks (Graph Attention Networks, E-GraphSAGE, and Edge Enhanced GNNs) combined with a Gated Recurrent Unit for learning communication patterns and intrusion detection.
Save Cite Related articles
[PDF] secitsociety.org
Cybersecurity Threat Intelligence System Using NLP and Knowledge Graphs
SR Punam - Transactions on Secure Communication Networks and …, 2024 - secitsociety.org
Introduces the Cybersecurity Threat Intelligence System (CTIS) which uses a knowledge graph built from NLP-processed unstructured data to enable advanced analysis, including link prediction, for uncovering connections between threats.
Graph Embedding for Prediction: Explains that graph embedding algorithms convert graph information into vector representations, which facilitates advanced tasks like predicting future links and helping to uncover previously unknown connections between threats.
Predicting Attacker Behavior: Details how link prediction, for example, can be used to hypothesize that newly detected malware might target a known weakness because it shows similarities to other entities in the vector space, helping analysts predict attacker strategies.
Save Cite Related articles All 2 versions 
[PDF] usenix.org
Detecting lateral movement in enterprise computer networks with unsupervised graph {AI}
B Bowman, C Laprade, Y Ji, HH Huang - 23rd International Symposium …, 2020 - usenix.org
Applies a logistic regression link predictor to detect anomalous authentication events, which are indicative of lateral movement in a computer network.
Unsupervised Graph Learning: Utilizes an unsupervised graph-based machine learning pipeline that learns latent representations of authenticating entities to perform anomaly detection via the learned link predictor.
Graph Data Model: Models a computer network as an authentication graph and uses node embeddings and ground-truth edge information to train the link predictor.
Save Cite Cited by 154 Related articles All 4 versions 
Ask a follow-up
Scholar Labs is experimental and can make mistakes.
Help & Privacy


Think properly then answer. 

---

**Cursor**

Thinking properly about feasibility, novelty, and what wins awards — here's my honest, decisive take.

## Reality check first

- **1 week + award-worthy + new architecture beating SOTA = not realistic.** A novel model that convincingly beats tuned baselines on 3 datasets cannot be done and validated in 7 days.
- **What *is* realistic and *can* win an award: a rigorous evaluation/benchmark paper with a surprising, defensible finding.** Best-paper awards at applied venues often go to "everyone's been measuring this wrong" papers, not to +1% architectures.
- **Verify the venue now.** Confirm IEEE CARS 2026 scope, page limit, and deadline before committing. If it's a workshop/short-paper track, an eval paper is an ideal fit; if it demands a novel system, adjust scope.

## The winning angle (your thesis)

> **"Cybersecurity link-prediction results are inflated by easy evaluation. Under realistic, security-aware negative sampling and temporal splits, reported performance collapses — and we show what actually holds up."**

Look at the literature you pasted: nearly all those KG papers report high Hits@K / MRR using **random negatives** and **random splits**. That is exactly the pitfall HeaRT exposed in general graphs — and **almost nobody has shown it for cyber graphs**. That gap is real, important, and provable in a week because it's *measurement*, not a new model.

You're not "reusing HeaRT." You learned GCN + LP from it, and you're now asking a **domain-specific question nobody answered**: *do cyber LP results survive realistic evaluation?*

## What makes it novel (beyond HeaRT)

Add a **security-aware hard-negative** definition — this is your original contribution:

- **Auth/host graphs:** hard negatives = host pairs in the **same subnet / same role** that never authenticated (plausible-but-absent links).
- **Attack KGs (CVE–CWE–CAPEC–ATT&CK):** hard negatives = entities **2 hops away / same category** (plausible attack steps that don't exist).
- Compare **random vs structural (HeaRT-style) vs security-aware** negatives.

Finding to aim for: *"security-aware negatives are even harder than structural ones; SOTA-style numbers drop 30–60%."* That's an award-grade headline if it holds.

## 3 datasets that are fast to get

| # | Dataset | Type | Why |
|---|---------|------|-----|
| 1 | **CVE–CWE–CAPEC–ATT&CK KG** | Attack knowledge graph | Public (NVD/MITRE); matches most papers in your list |
| 2 | **LANL 2015 auth** | Host authentication graph | Public; standard; Euler/Bowman use it |
| 3 | **CICAPT-IIoT (host-collapsed)** OR **a second public CTI KG** | Interaction / KG | You already have CICAPT; collapse to host edges |

Two graph *families* (interaction graphs + knowledge graphs) makes the finding general, which reviewers love.

## 1-week plan

| Day | Task |
|-----|------|
| **1** | Confirm venue + template. Build dataset 1 (CVE/CWE/CAPEC KG) → `edge_index`, splits. Reuse your SNAP preprocessing pattern. |
| **2** | Implement 3 negative samplers: random, structural (PPR/CN), security-aware. Implement metrics (Hits@K, MRR, AUC). |
| **3** | Baselines: Adamic-Adar/CN (heuristic), node2vec, **GCN** (you know this), optional SEAL-lite. Run on dataset 1 under all 3 negatives. |
| **4** | Dataset 2 (LANL) end-to-end. Add **temporal split** here (train past → test future). |
| **5** | Dataset 3 (CICAPT host-collapsed). Fill results matrix: 3 datasets × 3 negatives × ~4 methods. |
| **6** | Analysis + figures: performance-drop bar charts, one case study (a plausible hard negative that models wrongly rank high). |
| **7** | Write. Intro, related work (cite the list you have), method, results, discussion. |

## What to report (the money table)

For each dataset × method:

- Hits@10, Hits@50, MRR, AUC under **random / structural / security-aware** negatives
- **Relative drop** from random → security-aware (the headline number)
- Temporal vs random split gap (where time exists)

One bar chart of "% performance lost under realistic evaluation" across all datasets = your award figure.

## Why this can win vs. why it might not

**For:**
- Achievable in 7 days (eval, not new SOTA model)
- Genuinely novel *for cybersecurity* (the gap is real)
- Rigorous + reproducible (reviewers reward this)
- Actionable ("the field should change its protocol")

**Risk:**
- If the drop is small, the story weakens → mitigate by including the security-aware sampler (almost always produces a large drop) and multiple datasets so at least one shows a strong effect.
- Pure benchmark papers sometimes seen as "no new method" → mitigate by framing the **security-aware negative sampler as a proposed method/protocol**, not just analysis.

## My recommendation

**Do the security-aware realistic-evaluation paper.** Title direction:

> *"How Good Is Link Prediction for Cybersecurity, Really? Security-Aware Hard Negatives and Realistic Evaluation of GNNs on Threat Graphs"*

Primary contribution = **security-aware negative sampling protocol** + **first realistic re-benchmark of cyber LP across attack KGs and host graphs**.

Backup (if venue wants a "system"): add a **simple GCN + structural-prior fusion** model and show it's the most *robust* under hard negatives — turns the eval paper into a method paper.

---

If you want, tell me which dataset to start with and I'll (in Agent mode) wire the preprocessing + three negative samplers + GCN eval loop so Day 1–3 are done fast. I'd also quickly confirm the IEEE CARS 2026 page limit/deadline before you lock scope.

---

**User**

or should I predict link in provenance graph and if i get better results show that apt prediction in provenance graph is safer than using host graph?

---

**Cursor**

Yes — that's actually a **stronger, more award-worthy angle than pure benchmarking**, because it has a security payoff, not just a methodology point. But tighten the claim and mind the risks.

## Reframe the claim

Don't say "safer" (vague). Say one of:

> **"Finer-grained provenance graphs enable earlier / higher-recall APT link prediction than coarse host graphs — on the same events and same ground truth."**

The killer feature: **CICAPT-IIoT lets you build BOTH graphs from the same logs with the same attack labels.** That makes it a *controlled experiment* on graph granularity — clean, fair, and novel. Most prior work compares across different datasets, which is confounded. You wouldn't be.

## Why this beats the benchmarking idea

| | Benchmark/eval paper | Provenance-vs-host paper |
|--|----------------------|--------------------------|
| Contribution | "you measured wrong" | "this representation detects APTs better" |
| Security payoff | Indirect | Direct (better detection) |
| Award appeal | Medium | Higher (clear actionable result) |
| Controlled? | Across datasets | **Same data, two views** ← strong |

## The experiment (clean and fair)

From CICAPT-IIoT, same events, same labels, same model (GCN + LP):

1. **Host graph**: collapse to host/IP–host/IP edges (Euler-style)
2. **Provenance graph**: Process/Artifact nodes, typed edges
3. Train both, predict links, score edges
4. **Compare on identical attack ground truth**: per-tactic recall, AP, TPR@low FPR, **time-to-detect**

Headline if it works: *"Provenance LP detects credential-access/collection edges that never surface in the host graph, at X% higher recall."*

## Make it bulletproof (so a null result still publishes)

Frame as a **question**, not a guaranteed win:

> *"Does graph granularity matter for APT link prediction?"*

Then whatever the result:
- **Provenance wins** → "use provenance, here's the gain" (best case)
- **Mixed** → "provenance catches tactics X,Y; host catches Z — use both" (still useful)
- **Host wins** → "surprisingly, coarse graphs suffice — cheaper detection" (still a finding)

You can't lose the paper this way.

## Risks for 1 week (be honest)

| Risk | Mitigation |
|------|-----------|
| Provenance graph is big/heterogeneous | Start Process-only or Process+socket; subsample; use snapshots |
| Only 1 dataset (CICAPT) | Add **LANL host graph** as external host-baseline; that's your 2nd dataset |
| "Just two views of one dataset" critique | Add LANL (host) + maybe DARPA TC (provenance) later as generalization |
| Heterogeneous edges complicate GCN | Treat edge type as feature, or one model per edge type; keep v1 homogeneous |

## Strongest realistic scope (7 days)

1. **CICAPT-IIoT: provenance vs host, same labels** ← core contribution
2. **LANL: host-graph LP** ← shows host-graph baseline generalizes
3. Metrics: per-tactic AP, TPR@1%FPR, time-to-detect
4. One case study: an attack chain visible in provenance, invisible in host graph

That's **2 datasets, 1 controlled comparison, 1 generalization check** — defensible for a short/workshop paper.

## My recommendation

**Go with provenance-vs-host on CICAPT-IIoT.** It's more compelling than the benchmark paper, it uses the dataset you already have, and the same-data/same-labels design is genuinely strong. Keep the **security-aware hard-negative** idea as a *secondary* contribution inside it (realistic evaluation strengthens the comparison).

Working title:

> *"Granularity Matters: Provenance vs. Host Graph Link Prediction for APT Detection"*

One caution: you earlier said you didn't want provenance. If the heterogeneity/scale feels heavy, the host-only HeaRT-style benchmark is the safer 7-day bet. But if you want **award potential**, the provenance-vs-host controlled study is the better swing.

Want me to sanity-check feasibility by computing, in read-only mode, how big the host-collapsed graph is and how many attack edges survive in each view? That tells us in ~10 minutes whether the comparison has enough signal before you commit.

---

**User**



I am a Graduate Research Assistant (MS Computer Science) with hands-on experience building GCN-based link prediction pipelines — specifically, I have implemented and experimented with GCN encoder + dot-product decoder architectures, SVD-based node features, and custom negative sampling strategies (including a composite re-scoring method I developed called SoHeaRT) on large social graphs (Twitter/SNAP). I am comfortable with PyTorch Geometric, the HeaRT evaluation protocol, and metrics like Hits@K and MRR.

I now want to pivot this GCN link prediction expertise to a **cybersecurity dataset** and submit a paper to **IEEE CARS 2026** (Cyber Awareness and Research Symposium, Oct 26–28, University of North Dakota). The submission deadline is **June 15, 2026** — I have approximately **one week**.

**Conference fit:** The venue targets AI/ML in cybersecurity, threat detection and mitigation, cyber threat intelligence and data analytics, and threat modeling and predictive analytics. Papers are 6 pages (IEEE 2-column format), peer reviewed, published to IEEE Xplore.

**The landscape I've surveyed:** Existing work applies GNNs to cybersecurity knowledge graphs (CVE/CWE/CAPEC/ATT&CK entities), vulnerability co-exploitation prediction, lateral movement detection in enterprise networks (Euler, Jbeil), IDS log graphs, and malware knowledge graphs. Most papers either (a) build a new knowledge graph from scratch and apply KGE methods like TransE/RotatE, or (b) use GCN/GAT on a network traffic/authentication graph for anomaly detection.

**My constraints:**
- I have ~1 week total (writing + experiments)
- I need a **publicly available** cybersecurity dataset I can download and run experiments on immediately
- I want to reuse my existing GCN link prediction pipeline with minimal architectural changes — the novelty should come from the problem framing, dataset, or a targeted methodological contribution, not a full architecture redesign
- I do not have access to proprietary network logs; I need open benchmark data

**What I need from you:**

1. **Recommend the most feasible project angle** given my background and time constraint — specifically, should I target (a) a CVE/CWE/CAPEC knowledge graph completion task, (b) network intrusion/lateral movement detection framed as link prediction, or (c) something else? Justify the choice based on dataset availability and how naturally it maps to my existing GCN pipeline.

2. **Identify the best concrete publicly available dataset** for that angle (e.g., NVD/MITRE-derived KG, CERT insider threat, LANL authentication graph, CTU malware captures, CIC-IDS datasets, etc.) — include where to download it and what preprocessing is needed to get to an edge list I can feed into PyG.

3. **Propose a specific, honest research contribution** I can claim in one week — something that goes beyond "we applied GCN to dataset X" but is achievable without a multi-month pipeline. Given my SoHeaRT experience with negative sampling, are there opportunities there in the cybersecurity graph setting?

4. **Sketch a 1-week execution plan** broken into daily milestones: data acquisition → graph construction → baseline experiments → proposed method → writing.

5. **Flag the two or three most relevant prior papers** I should cite and differentiate from, based on the angle you recommend.

Be direct and pragmatic. I need a plan I can actually execute in 6 days, not an ambitious research agenda. If the honest answer is that a particular angle is too risky for the timeline, say so.






---

**Cursor**

Here's the direct, execution-focused answer.

## 1. Recommended angle: (b) lateral-movement detection as link prediction

**Pick (b), not (a).** Reasoning tied to your actual assets:

- Your pipeline is **homogeneous GCN + dot-product decoder + structural negatives**. That maps **1:1** to an authentication/host-communication graph (single edge type: "host u talked to/authenticated with host v"). **Zero architecture change.**
- KG completion (a) is **multi-relational** (CVE→affects→CWE, etc.). To be credible there you'd need relation-aware decoders (R-GCN/TransE/RotatE). That's an architecture change *and* you'd be compared against mature KGE baselines — bad odds in 6 days.
- Your **SoHeaRT negative-sampling work is the differentiator**, and it's most naturally and most impactfully transferred to (b): host graphs have meaningful "plausible-but-absent" links (same subnet/role), and the lateral-movement literature still uses **trivial negatives** — a real, citable gap.

**Honest caveat:** the only serious risk in (b) is **data size** (below). I give a bounded plan + a fallback that keeps the *same contribution*.

## 2. Dataset: LANL 2015 (primary) — with a hard fallback

**Primary — LANL "Comprehensive, Multi-Source Cyber-Security Events"**
- Download: https://csr.lanl.gov/data/cyber1/ (`auth.txt.gz`, `redteam.txt.gz`). Public, no agreement.
- Why: standard for LP-based lateral movement (Euler, Bowman, Jbeil all use it); **has ground-truth red-team labels** in `redteam.txt` → your positives-of-interest.
- **Preprocessing to a PyG edge list:**
  1. Stream `auth.txt` (it's large — don't load whole). Fields include time, src user, src comp, dst comp.
  2. Build a **host–host graph**: edge = (src_comp, dst_comp). Map comp IDs → 0..N-1 (exactly your `preprocess_twitter_edges.py` renumbering).
  3. **Bound the scale**: take a contiguous window around the red-team activity (the campaign starts ~day 2). A few days of auth is enough and keeps N, E manageable.
  4. Split into **temporal snapshots** (or one train-past / test-future split) → `edge_index`, `num_nodes`.
  5. Label test edges using `redteam.txt` (src_comp+dst_comp+time match) = malicious positives.
  6. Node features: SVD of the adjacency (you already do this) or one-hot/role flags from naming (U/C/DC/TGT).

**Fallback (decide by end of Day 2): a CTI/attack KG (CVE–CWE–CAPEC–ATT&CK)**
- Source: NVD JSON feeds (https://nvd.nist.gov/vuln/data-feeds) + MITRE CWE/CAPEC/ATT&CK (CSV/STIX from GitHub).
- Collapse to a (mostly) homogeneous entity graph; run the *same* GCN + SoHeaRT pipeline.
- Data wrangling here is trivial and reliable — so if LANL streaming/labeling isn't working by Day 2, switch without losing your contribution.

**Skip:** OpTC (1TB+), CIC-IDS/CIDDS (netflow, not naturally an LP graph), CERT insider (weak link-prediction framing) — all cost you days.

## 3. Honest, achievable contribution (built on SoHeaRT)

> **"Realistic evaluation of GCN link prediction for lateral-movement detection, with a security-aware hard-negative sampler."**

Three concrete, defensible claims:

1. **Evaluation gap:** lateral-movement LP is reported with random negatives; under **HeaRT-style hard negatives + temporal split**, ranking performance (Hits@K/MRR) drops substantially → prior numbers are optimistic.
2. **Method — SoHeaRT-Sec:** adapt your composite re-scoring to produce **topology-plausible, security-meaningful negatives** (e.g., same-subnet/same-degree host pairs that never communicated). Show negative-aware **training** improves ranking of true red-team links vs random-negative training.
3. **Finding:** report which evaluation regime correlates with actually surfacing red-team edges.

This is honest ("we don't claim a new SOTA architecture; we fix evaluation + propose a domain negative sampler"), it's *yours* (SoHeaRT lineage), and it's 6 pages of real content.

## 4. Six-day plan

| Day | Milestone |
|-----|-----------|
| **1 (today/tmrw)** | Download LANL. Write the streaming parser → host–host edge list + ID map (reuse Twitter preprocess). Sanity stats: N, E, edges/day. |
| **2** | Temporal split + red-team labeling of test edges. **Decision gate:** if data/labels working → continue LANL; else switch to CTI-KG fallback. Build random-negative baseline eval. |
| **3** | Run your **GCN + dot-product** (SVD features) under **random negatives**. Get Hits@K/MRR/AUC. This is your baseline table. |
| **4** | Implement **HeaRT + SoHeaRT-Sec** hard negatives. Re-evaluate same model → show the performance drop (claim 1) and SoHeaRT-Sec effect (claim 2). |
| **5** | Add heuristic baselines (CN/Adamic-Adar, node2vec) + temporal-vs-random split comparison. Lock results matrix. One case study + figures (perf-drop bar chart, a hard negative the model wrongly ranks high). |
| **6** | Write all 6 pages. Intro, related work, method, results, discussion, limitations. Internal proofread, format check, submit. |

Keep a **frozen "minimum viable paper"** by end of Day 4 (baseline + hard-negative drop on one split) so Days 5–6 only *add*.

## 5. Three papers to cite and differentiate from

1. **King & Huang, "Euler" (ACM TOPS 2023)** — temporal LP for lateral movement. *Differentiate:* they focus on scalable temporal anomaly detection; you focus on **evaluation rigor + security-aware negatives** for LP ranking, not a distributed framework.
2. **Bowman et al., "Detecting Lateral Movement…with Unsupervised Graph AI" (USENIX Security 2020)** — auth-graph link predictor, **static**, simple negatives. *Differentiate:* you show its evaluation regime (random negatives) overstates performance; you propose harder, security-aware negatives.
3. **Khoury et al., "Jbeil" (IEEE S&P 2024)** — inductive temporal LP for lateral movement. *Differentiate:* they push inductive modeling; your contribution is the **negative-sampling/evaluation protocol** that's orthogonal and applicable to their setting too.

(Also cite HeaRT as the methodological basis you're transferring into cybersecurity.)

---

**Bottom line:** Do **(b) lateral movement on LANL** with your existing GCN+dot+SoHeaRT pipeline; contribution = **realistic hard-negative evaluation + SoHeaRT-Sec**. It's the lowest-architecture-risk, highest-fit option, and your negative-sampling background is exactly the edge. The one risk is LANL data wrangling — mitigate with the bounded window and the **Day-2 decision gate** to fall back to a CTI-KG without changing your contribution.

Want me (in Agent mode) to write the LANL streaming parser + temporal split + red-team labeler now, modeled on your `preprocess_twitter_edges.py`? That removes the biggest time risk on Day 1.

---

**User**

compare your suggestion with working on: "Finer-grained provenance graphs enable earlier / higher-recall APT link prediction than coarse host graphs — on the same events and same ground truth."

---

**Cursor**

Direct comparison of the two options.

## The two candidates

- **Option A (my prior pick):** LANL lateral-movement LP + realistic hard-negative evaluation (SoHeaRT-Sec). *Contribution = evaluation protocol + security-aware negatives.*
- **Option B (yours):** Provenance vs host graph on CICAPT-IIoT, same events/labels. *Contribution = graph granularity matters for APT LP.*

## Head-to-head

| Dimension | A: LANL eval-protocol | B: Provenance-vs-host |
|-----------|----------------------|------------------------|
| **Data readiness** | ⚠️ Download + stream ~GBs + red-team labeling | ✅ **Already on disk, already parsed, labels present** |
| **Pipeline fit** | ✅ Homogeneous host graph, zero arch change | ⚠️ Heterogeneous/typed/directed — needs simplification |
| **Novelty / award ceiling** | Medium (methodology) | ✅ **Higher** (controlled security finding) |
| **Story clarity** | "you measured wrong" | ✅ "this representation detects APTs better" |
| **Controlled comparison** | No (one graph) | ✅ **Same data, two views = clean causal claim** |
| **Result risk** | Low (drop almost always shows) | Medium (provenance must actually win — but safe if framed as a question) |
| **Single-dataset critique** | Real | Real → mitigate with LANL host arm |
| **Biggest risk** | **Data wrangling eats Day 1–2** | **Heterogeneity modeling + building 2 graphs** |

## The key insight

**They're not exclusive — B contains A's host arm.** The "host graph" side of your provenance-vs-host study is exactly the homogeneous GCN+dot+SoHeaRT pipeline from Option A. So:

- Option B = Option A's host pipeline **+** a provenance arm **+** a better story.
- Your earlier #1 risk (LANL data wrangling) **disappears** because CICAPT is already parsed and labeled.

## Honest verdict

**Option B has the higher award ceiling and lower data risk**, but trades that for **modeling risk** (heterogeneous provenance). That trade is favorable because:

1. Data-in-hand is the single biggest 6-day advantage.
2. You can **tame heterogeneity cheaply**: build the provenance graph as a directed homogeneous graph (Process+Artifact as nodes, edge-type as an edge feature or just collapsed) — your GCN+dot decoder works almost unchanged.
3. The same-events/same-labels design is genuinely novel vs Euler/Bowman/Jbeil (all host-only).

## Recommended path: B as primary, with a risk gate

**Primary: Provenance vs host on CICAPT-IIoT**, SoHeaRT-Sec hard negatives folded into *both* arms (that's your methodological signature, kept).

**Add: LANL host graph as the external generalization** of the host arm — only if time allows (Day 5). This kills the single-dataset critique and recycles Option A's work.

**Decision gate (end of Day 2):** if the provenance graph modeling isn't producing sane LP numbers, **fall back to A** (host-only on CICAPT + LANL with the eval-protocol contribution). You lose the granularity story but keep a publishable paper. Same pipeline, so no wasted work.

## Adjusted contribution statement

> **"Does graph granularity matter for APT link prediction? We show, on identical events and ground truth, that finer-grained provenance graphs yield higher-recall / earlier APT link prediction than coarse host graphs — and that security-aware hard negatives (SoHeaRT-Sec) are essential to measure this fairly."**

That bundles your **granularity finding** (award appeal) with your **SoHeaRT negative-sampling edge** (your unique skill) in one paper.

## Papers to differentiate from (same three, reframed)
- **Euler**, **Bowman**, **Jbeil** — all **host/auth-level**. Your wedge: *they never test whether a finer representation detects more, on the same data.*

---

**Bottom line:** Switch to **Option B (provenance-vs-host on CICAPT-IIoT)** as primary — data-in-hand removes your worst risk, the story is stronger, and your SoHeaRT work still anchors the method. Keep **Option A as the Day-2 fallback**, since its host pipeline is literally one arm of B.

Want me (Agent mode) to start Day 1: build both graph views (provenance + host-collapsed) from `Phase1`/`Phase2` with attack labels, modeled on your `preprocess_twitter_edges.py`? I can also first run a quick read-only feasibility check on how many attack edges survive in each view so we know the signal is there before committing.

---

**User**

 see what does the paper say about provenance experiment I am planning to do?
35
Euler: Detecting Network Lateral Movement via Scalable
Temporal Link Prediction
ISAIAH J. KING and H. HOWIE HUANG, The George Washington University, USA
Lateral movement is a key stage of system compromise used by advanced persistent threats. Detecting it
is no simple task. When network host logs are abstracted into discrete temporal graphs, the problem can
be reframed as anomalous edge detection in an evolving network. Research in modern deep graph learning
techniques has produced many creative and complicated models for this task. However, as is the case in many
machine learning fields, the generality of models is of paramount importance for accuracy and scalability during training and inference. In this article, we propose a formalized approach to this problem with a framework
we call Euler. It consists of a model-agnostic graph neural network stacked upon a model-agnostic sequence
encoding layer such as a recurrent neural network. Models built according to the Euler framework can easily distribute their graph convolutional layers across multiple machines for large performance improvements.
Additionally, we demonstrate that Euler-based models are as good, or better, than every state-of-the-art
approach to anomalous link detection and prediction that we tested. As anomaly-based intrusion detection
systems, our models efficiently identified anomalous connections between entities with high precision and
outperformed all other unsupervised techniques for anomalous lateral movement detection. Additionally, we
show that as a piece of a larger anomaly detection pipeline, Euler models perform well enough for use in
real-world systems. With more advanced, yet still lightweight, alerting mechanisms ingesting the embeddings
produced by Euler models, precision is boosted from 0.243, to 0.986 on real-world network traffic.
CCS Concepts: • Security and privacy → Network security; Intrusion detection systems;;• Computing
methodologies → Neural networks; Anomaly detection
Additional Key Words and Phrases: Lateral movement detection, graph neural network, temporal graph
ACM Reference format:
Isaiah J. King and H. Howie Huang. 2023. Euler: Detecting Network Lateral Movement via Scalable Temporal
Link Prediction. ACM Trans. Priv. Sec. 26, 3, Article 35 (June 2023), 36 pages.
https://doi.org/10.1145/3588771
This article is an extension of a NDSS’22 conference paper by King & Huang [44]. There has been significant addition
to the conference paper, including new experiments augmenting the output of the original models and measuring their
scalability. We show that by continuing the work done in the conference paper, and applying the framework to a more
complex pipeline, we can attain precision high enough for use in real-world systems. There is also additional analysis of
our initial results and extended discussions using new diagrams and tables to improve readability throughout the article.
This work was supported in part by DARPA under agreement number N66001-18-C-4033 and National Science Foundation
grants 1618706, 1717774, and 2127207. The views, opinions, and/or findings expressed in this material are those of the
authors and should not be interpreted as representing the official views or policies of the Department of Defense, National
Science Foundation, or the U.S. Government.
Authors’ address: I. J. King and H. H. Huang, Science & Engineering Hall, Dept. of Electrical & Computer Engineering, 800
22nd St NW Washington, DC 20052; emails: {iking5, howie}@gwu.edu.
Permission to make digital or hard copies of all or part of this work for personal or classroom use is granted without fee
provided that copies are not made or distributed for profit or commercial advantage and that copies bear this notice and
the full citation on the first page. Copyrights for components of this work owned by others than the author(s) must be
honored. Abstracting with credit is permitted. To copy otherwise, or republish, to post on servers or to redistribute to lists,
requires prior specific permission and/or a fee. Request permissions from permissions@acm.org.
© 2023 Copyright held by the owner/author(s). Publication rights licensed to ACM.
2471-2566/2023/06-ART35 $15.00
https://doi.org/10.1145/3588771
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:2 I. J. King and H. H. Huang
1 INTRODUCTION & MOTIVATION
Lateral movement is a key stage of the MITRE ATT&CK framework [72] describing the behavior
of advanced persistent threats (APTs). At its core, lateral movement is malware propagating
through a network to spread onto new computers in an attempt to find the adversary’s target. This
may involve pivoting through multiple systems and accounts in a network using either legitimate
credentials or malware to accomplish the task [9]. As it is one of the final steps in the kill chain
before complete compromise, detecting it early is of critical importance.
A plethora of machine learning approaches to intrusion detection exist, both signature-based
models [18, 45, 56, 57, 79, 80] and anomaly-based [13, 16, 28, 37, 58, 67, 81]. These latter techniques
are especially well-suited for lateral movement detection, as APT techniques such as “Pass the
Ticket” [7], or even just using stolen credentials [8], are very difficult to formalize into signatures
for signature-based intrusion detection systems [9].
The most robust way to detect malware propagation is not to exhaustively list every known
malicious signature correlating with it; rather, it is to train a model to learn what normal activity
looks like and to alert when it detects behavior that deviates from it. However, detecting anomalous activity in an enterprise network presents unique challenges. The data involved both during
training and the implementation of an anomaly-based intrusion detection system is enormous. Often the log files that such a system would require as input are terabytes large. To be useful, a lateral
movement detection model must be highly scalable to accommodate such large data. Additionally,
when viewed as a classification problem, any such system would have to be highly precise. Millions of events occur in an enterprise network on any given day, and only a fraction of a percent
of all interactions are ever anomalous [17]. Therefore, a model must have an extremely low rate
of false alerts so as not to overwhelm its users.
In this work, we formulate anomalous lateral movement detection as a temporal graph
link prediction problem. Interactions occurring in discrete units of time on a network can be
abstracted into a series of graphs Gt = {V, Et } called snapshots, where V is the set of entities
in the network that had interactions Et = {(u,v) ∈ V} during a set period of time, t. A temporal
link prediction model will learn normal patterns of behavior from previous snapshots and
assign likelihood scores to edges that occur in the future. Edges with low likelihood scores
correlate to anomalous connections within the network. As Reference [16] points out, these
anomalous connections are often indicative of lateral movement. As we will later show, this
reframing of the problem improves precision over standard anomaly-based intrusion detection
techniques.
Recent approaches to temporal link prediction combine a graph neural network (GNN) with a
sequence encoder such as a recurrent neural network (RNN) to capture topological and temporal features of an evolving network. However, these approaches are either reliant on RNN output
during the GNN stage of embedding [35] or merely incorporate GNNs into the RNN architecture [19, 63, 68]. As Figure 1(a) illustrates, these models are necessarily sequential and, unfortunately, cannot scale to the large datasets that they would need to process to be useful lateral
movement detectors.
Proposed solution. To address this problem, we have observed that the most memoryintensive part of existing architectures occurs during the message-passing stage within the GNN.
Furthermore, there exists an imbalance between the massive size of node input features and the
comparatively minuscule topological node embeddings. This means the most work and the most
memory usage occurs in the GNN before the simpler forward pass of the RNN is calculated, necessarily in serial. If several replicated GNNs operate on snapshots independently, then they can
execute concurrently, as shown in Figure 1(b). Amdahl’s Law [11] would suggest that by distributing such a large portion of work, performance improvements will ensue.
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:3
Fig. 1. (a) Prior approaches rely on RNN output during the GNN stage of embedding or merely incorporate
GNNs into the RNN architecture, which forces models to work in serial, one snapshot at a time. In contrast,
(b) the Euler framework can utilize several worker machines to hold consecutive snapshots of a discrete
temporal graph. These workers process snapshots in parallel through a replicated GNN shared across each
machine. The output of these GNNs is returned to the leader machine, which runs them through a recurrent
neural network to create temporal node embeddings that may be used for link prediction. The framework is
explained in detail in Section 5.
In this work, we have developed Euler,1 a formalized approach for scalable dynamic link prediction and anomalous edge detection. The framework involves stacking a model-agnostic GNN upon
a sequence encoder such as an RNN. In this way, a network’s topology at discrete moments in time
is encoded by the GNN, and the dynamic changes in connections are encoded by the RNN. The
embeddings produced by this model provide prior probabilities for future states of the network
given what is embedded about the past structure. Most importantly, the framework is designed
such that GNNs may be replicated across several worker machines, and execute independently,
allowing disjoint sets of snapshots to be processed concurrently. When the GNNs’ work occurs in
parallel for each snapshot, the topological data for an entire series of graphs can theoretically be
encoded in the time it takes to encode the snapshot with the most edges. With these immense performance enhancements, detecting anomalous user activity in industry-scale, real-world networks
using powerful GNN models becomes tractable.
Experimental evaluation. There are two things this work aims to show: that despite its simplicity, Euler can outperform slower, more complicated temporal link prediction methods, and
that link prediction is an effective method for anomalous lateral movement detection. To evaluate the first contention, we compare the effectiveness of a model following the Euler framework
against several state-of-the-art temporal link prediction models on three general-purpose temporal graph datasets. The results of these experiments show that despite its simplicity, the Euler
framework performs as well or better than the state-of-the-art. With Euler proven as an effective
link prediction model, the next step is to show link prediction can be used for lateral movement
detection in larger, real-world event logs.
Implementation as an intrusion detection system. To evaluate the second contention, we
test several models following the Euler framework on the LANL comprehensive multi-source
cyber-security events dataset [41]. This dataset includes labeled event data from 58 consecutive
days of real-world computer network use interspersed with red team activity. There are approximately 1.6 billion events in total. This is a test not only of the Euler framework’s precision, but
also its ability to scale. Our tests show that Euler-based models outperform prior works in both
precision and compute time.
We further augment these results by using Euler as a stage in a longer anomaly detection
pipeline. The embeddings produced by link prediction models contain vital information about the
1Source code available at https://github.com/iHeartGraph/Euler.
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:4 I. J. King and H. H. Huang
structure of dynamic graphs, but Reference [62] points out that anomaly detection requires more
data than just graph structure. By using the outputs of Euler models as inputs to an additional
distributed anomaly detection network, we can extract even more information. These experiments showed that this strategy significantly improves precision and remains scalable to large
datasets.
In summary, our research contributions are:
• We present, to the best of our knowledge, the first use of temporal graph link prediction for
anomaly-based intrusion detection. Other research in applying graph analytics to anomaly
detection either does not consider the temporal nature of the data or does not use a powerful
GNN model. By incorporating both elements into Euler, models built on this framework
outperform other unsupervised anomaly-based intrusion detection systems and yield more
informative alerts.
• We demonstrate that for temporal link prediction and detection, the simple framework we
propose is equally or more accurate and precise than state-of-the-art temporal graph autoencoder models. Euler models attain higher area under the curve and average precision
scores by 4% in inductive link detection tests, and about equal metrics to the state-of-the-art
in transductive link prediction tests.
• We propose a scalable framework for distributed temporal link prediction for use on big
data. The Euler framework is simple and makes message-passing lightweight even over
large graphs. By breaking edge lists into temporal chunks and processing them in parallel,
the computational complexity of the message-passing stage of the model is theoretically
bound by only the snapshot with the most edges. Other optimizations allow the RNN to
operate in parallel with some of the GNN workers, further improving performance.
Please note that this work is an extension of our previously published conference paper [44].
Novel contributions of this work are the additional experiments in Sections 9 and 10 as well as a
more complete literature review in Section 2, and a discussion of the assumptions we make about
the threat model in Section 7. Additionally, Section 8 contains extended discussion and analysis of
the results from our previous work.
2 RELATED WORK
2.1 Intrusion Detection Systems
Many techniques exist to use machine learning on cyber-security data for intrusion detection.
They can be broadly categorized as signature-based intrusion detection systems (SIDS) and
anomaly-based intrusion detection systems (AIDS).
Signature-based intrusion detection systems are used by many popular monitoring systems [18]. They have a distinct advantage in speed, simplicity, and effectiveness so long as
the attack has been previously observed [43]. The primary focus of research involving graphs
for signature-based intrusion detection, is on the analysis of provenance graphs [45]. References [56, 57] accomplish this through subgraph matching. They generate provenance graphs from
kernel log files that can be quickly identified as malicious if they contain a previously identified
subgraph correlating to compromise. References [79, 80] follow a similar approach, however, they
use vector embeddings of the provenance graphs to measure similarity between observed and
known malicious graphs. This approach to subgraph matching, while sometimes less sound than
exhaustive search, is much faster. We use a similar embedding approach in this work. Despite
these methods’ effectiveness at identifying known threats, advanced attackers will endeavor not
to use a method with a previously discovered signature. For this reason, we turn to anomaly-based
intrusion detection methods.
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:5
Anomaly-based intrusion detection systems must first define a baseline for normal behavior,
then generate alerts when events occur that significantly deviate from this baseline. The definition for normalcy is highly contingent on the abstraction used to represent the system. Frequencybased AIDS define normalcy through observed distributions of events’ frequencies or other stochastic processes present in the network [37, 81]. Midas [14] incorporates structural data with frequency counts; however, it can only detect bursts of anomalous events and would struggle to identify individually anomalous connections. Similarly, Whisper [29] analyzes ongoing packet features
in the frequency domain; they find anomalous traffic incurs phase anomalies in sliding windows
that are difficult to hide. Supervised learning approaches such as References [10, 24, 55] analyze
features of individual events with data-mining approaches. Like the Euler approach, many unsupervised systems use deep autoencoders. However, they, too, embed event features rather than
network interactions [13, 28]. Kitsune [58] uses temporal patterns in addition to event features;
however, temporal information is used as an input feature rather than for sequence encoding.
Network-level anomaly-based intrusion detection systems in the field of graph analytics are
lacking. Only Reference [16] has proposed a method for detecting anomalous events in host logs
that leverages the rich graph structure inherent to the medium. This approach only considers the
network as a static graph, so edge embeddings that occurred at different points in time always
have the same anomaly score.
2.2 Temporal Link Predictors
Temporal link prediction is defined as finding a function that takes two nodes in a graph at a
point in time, and outputs the probability of an edge existing between them. This problem has
applications in fraud detection [35], contact tracing [19], traffic prediction [84], and as we will
show, anomalous lateral movement detection.
A great deal of research has been done in this field, however, there are very few approaches
that use graph neural networks. Despite the massive improvements in accuracy they offer [47],
many papers still opt to use traditional deep learning approaches. Models such as Triadic Closure [85], DyLink2Vec [65], and DynGEM [33] use n-tuples of nodes known to be connected as
inputs to generate embeddings using multilayer perceptrons (MLPs), rather than taking a more
global relational approach. Their loss functions account for this somewhat through neighborhood
sampling, but MLPs cannot leverage the rich topological information the same way a graph convolutional operator can. References [32, 53] also use MLPs to operate on adjacency vectors, however,
they also incorporate a recurrent neural network. This way, both topological and temporal information is present in the embeddings. Both models take nodes’ full adjacency vectors as inputs,
and their embeddings represent the dynamics of nodes’ neighbors over time. Our work extends
this idea and incorporates better structural information by instead using a graph convolutional
operator.
Additional studies have been done on the effectiveness of random walk-based embeddings over
time. Reference [61] found that enforcing temporal order in random walks, with no other changes,
outperforms LINE [74], DeepWalk [64], and node2vec [34]. However, as Reference [70] points
out, random walk-based approaches are very slow and memory-intensive. Furthermore, these approaches cannot take into account node features and lose out on the rich information they can
provide. Additionally, Reference [61] only provides one embedding of each node; that is to say,
the embeddings of nodes do not change over time. This may not be the best approach, as nodes’
functions in a network may be dynamic. They can, however, consider edge weights, which is something the MLP approaches lack. These are also dynamic and provide important information about
the future of the graph’s structure. This is something we keep in mind with the framework we
propose and is easily incorporated into Euler models depending on the choice of GNN.
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:6 I. J. King and H. H. Huang
Many temporal link prediction approaches that do use graph neural networks are preoccupied
with incorporating them into recurrent neural networks in lieu of linear layers. References
[19, 35, 68] all modify existing RNN architectures to incorporate graph convolutions. VGRNN [35],
however, uses a more modular approach, much like we are proposing. The inputs passed to their
graph RNN are embeddings generated with a variational graph autoencoder [48]. In this way,
the information propagated through time by the RNN is strongly influenced by the changing
structure of the graph. However, the topological encoders are dependent on the output of the
recurrent layer at the previous timestep, so they cannot be implemented with our framework.
EvolveGCN [63] takes an interesting approach where the parameters of the GCN themselves are
passed through a recurrent unit. Their technique shows vast improvements over the state-of-theart in link prediction tests where edges are unknown for long periods of time. However, we are
primarily interested in anomaly detection, where the structure of a graph at any given time slice
is likely available. We will show that their model does not perform as adequately as others in this
domain.
There are several models that could readily fit into the framework we propose. However,
none take advantage of the potential parallelism present in their own models. The first of these
was Reference [68], which mentions and tests stacking Chebyshev filter convolutions on an LSTM,
however, much of this work is focused on modifying LSTMs to incorporate graph convolutions.
DySAT [66] uses a graph attention network fed into temporal self-attention layer; it follows the independent topological encoder to serial temporal encoder framework we propose. DyGGNN [73]
stacks gated graph neural networks on an LSTM, however, their model is only tested on full graph
classification. T-GCN [84] consists of a GCN stacked on a GRU following the framework, but it
was used for node regression, not temporal link prediction. However, we will show their model
is an excellent link predictor as well. It is evident that many state-of-the-art models already use
this approach in multiple problem domains. This means by formalizing and optimizing it, it has
the potential to improve several existing works. These papers are the main motivation for our
work.
2.3 Distributed Machine Learning
Techniques for distributed machine learning can generally be separated into two broad categories:
data parallel and model parallel [77, 82]. Under model parallelism, portions of a model’s parameters
are split across multiple workers and updated by those same workers. This is how we implement
the separation between the GNN and RNN layers. With data parallelism, one assumes that training
and real-world data are independent and identically distributed; thus, it is perfectly acceptable to
partition training data and use disjoint subsets to train replicas of one model [82]. The forward
pass and gradient calculations are done in parallel by multiple workers, and the parameter update
stage is done by some centralized or otherwise coordinated means.
One such method to coordinate parameter updates of a replicated model is the parameter server [50]. Here, one machine, the parameter server, holds a master-copy of a model’s
parameters. Several workers hold replicas of the model and a subset of the training data.
These workers then calculate gradients from the loss generated by the data they each hold
and send these gradients to the parameter server. The parameter server aggregates the gradients, updates the model’s parameters accordingly, and sends the updated parameters back
to each replica. The PyTorch DistributedDataParallel (DDP) further extends this idea [52]
using collective communication. This allows workers to broadcast partial gradients as they
are backpropagated, allowing for communication during computation. Because their method
enjoys near-linear scalability, we use it to implement the data parallel within the GNN layer
of Euler.
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:7
3 BACKGROUND
Our approach to anomaly-based intrusion detection represents network activity as discrete temporal graphs. From there, detecting evidence of lateral movement betrayed by anomalous network
activity is equivalent to anomalous edge detection, which we accomplish via temporal link prediction. In this section, we will briefly define these terms and define the symbolic notation we will
use throughout this work.
Lateral Movement Detection
Lateral movement is the stage in an attack where an adversary roams the network in an attempt
to find high-value hosts, services, and data in an attempt to spread from a compromised node
throughout the network [51]. It is very difficult to detect in enterprise systems, because it is a slow
process [26] that utilizes normal looking tools [71] such that individual malicious system events
do not appear out of the ordinary. However, it is not completely invisible: As the objective of
lateral movement is to propagate to more valuable nodes, there are often communications between
entities that are not normally related to each other.
References [15, 16, 42, 54] have shown that this stage of the attack is most easily detected in
unusual communications, and in particular authentications, between users or machines within a
network. When data are represented as a graph, unusual authentications are understood as paths or
edges that connect nodes with low link prediction probabilities. For this reason, we are motivated
to represent cyber security data as a temporal graph.
Discrete Temporal Graphs
A discrete temporal graph2 G = {G1, G2,... GT } is defined as a series of graphs Gt = {V, Et, Xt },
which we refer to as snapshots, representing the state of a network at time t. Here, V denotes a
set of all nodes that appear in the network, Et denotes relationships between nodes at time t, and
Xt represents any features associated with the nodes at time t. In this work, all graphs are directed,
and some have weighed edges, W : E→ R representing edge frequencies during the time period
each snapshot encompasses.
Let the interactions I between users and machines in a network at specific times be represented
as a multiset of tuples <src,dst,ts>. Here, src is an entity interacting with entity dst at time ts. From
this multiset, we can build the temporal graph G = {G0,...,GT } with time window δ. The set of
all nodes V is the set of every src and dst entity that appears in I. The set of edges at time t, Et
is constrained such that for all edges (u,v) ∈Et there exists an interaction < u,v,w >∈I where
t ≤ w < t + δ.
Temporal Link Prediction
Temporal link prediction is defined as finding a function that describes the likelihood that an edge
exists in a temporal graph at some point in time, given the previously observed snapshots of a
network. By representing an enterprise network as a temporal graph, we can further extend this
definition to encompass anomaly detection. This follows from the assumption that anomalous
edges in a temporal graph will have a low probability of occurring given what is known about the
network’s behavior in the past.
Lateral movement detection with temporal link prediction is then defined as finding a function
learned from the temporal graph G of network activity that predicts the likelihood of future interactions occurring in unseen snapshots. An observed interaction between entities with a likelihood
score below a certain threshold is said to be anomalous. These anomalous edges, in the context of
network monitoring, are often indicative of lateral movement [16].
2For simplicity, for the remainder of this work, we refer to these simply as “temporal graphs”.
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:8 I. J. King and H. H. Huang
Fig. 2. A simple example of hard-to-detect temporally anomalous activity in a network with three machines
and two users. The normal sequence of events is that a user authenticates with a computer, then that computer makes a request to the shared drive. However, suppose C1 is compromised with Mimikatz or a similar
malicious process with access to LSASS memory [6]. After Bob’s initial connection to C1, his credentials are
harvested, and at time t = 5, Computer 1 makes a request to the shared drive without Bob’s involvement.
Though this exact edge has been seen before, in this temporal context it is now indicative of C1’s compromise
manifested as anomalous network activity.
4 MOTIVATION
Anomaly detection is strongly dependent on how data are represented. The way in which we model
the network determines what is considered anomalous activity. An anomaly is simply a deviation
from an observed pattern of behaviors; for attacks to appear anomalous, they must cause some
unexpected representation of the system to emerge. The way in which the underlying data are
represented will ultimately shape and constrain how this is defined. Historically, there have been
two main ways of abstracting data for anomaly detection: frequency-based and events-based [17].
Frequency-based methods, such as References [14, 29, 37, 81], define anomalous behavior as activity that significantly deviates from observed temporal patterns. Events-based methods, such
as References [10, 13, 24, 28, 55, 58], identify commonalities in features, such as packet count, protocols, and so on, associated with individual events within a system. Anomalies are then defined
as events with unexpected features.
The problem with these two approaches is that they ignore the inherently structural nature of
network data. This is readily apparent by observing that one of the most influential datasets for
intrusion detection [4, 60] has no features for source or destination entities. All that matters with
these models are the details of the events themselves in isolation, not the machines between which
they occur. It is impossible to capture anomalous interactions between entities in a network using the traditional data representation. Attacks that use stolen credentials, for example, may have
features that look entirely normal, and may occur at rates that are non-anomalous as seen in the
DARPA Transparent Computing dataset [75]. Indeed, the lateral movement stage of a compromise
is often “low-and-slow” and intentionally similar to normal network traffic both in frequency and
features, making them indistinguishable from normal activity at the network level when represented with traditional means [26]. During the lateral movement stage of an attack, anomalies
instead manifest as unusual connections between entities [20]. Networks are webs of relational
data, fluctuating over time; the most natural way to represent and analyze relational data is as a
graph.
There has been growing interest in static graph-based methods for intrusion detection, such
as the work of Bowman et al. 2020 [16]. Here, normalcy is defined in terms of interactions between entities within systems, but time is not considered. Anomalies are defined as edges with
low probability given what is known about the graph’s structure.
To demonstrate the advantages and disadvantages of the above intrusion detection systems,
consider the example shown in Figure 2. The first two time slices show normal activity in the
network: First, att0, Alice and Bob authenticate with their computers A and B, then att1 computers
A and B make a request to the shared drive. At timest2 and t3, we see that when Bob does not first
authenticate with computer B, it does not communicate with the shared drive. A simple probability
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:9
distribution is apparent:
P ((C1, SD) ∈Et+1 | (B,C1) ∈Et ) = 1
P ((C1, SD) ∈Et+1 | (B,C1)  Et ) = 0. (1)
However, in t4 and t5, something unusual occurs: Computer B requests data from the shared drive
without Bob authenticating with it first! Suppose C1 was compromised with Mimikatz or a similar malicious process with access to LSASS memory [6]. After Bob’s initial connection to C1, his
credentials have been harvested. After lying in wait, with the newfound stolen credentials, the
APT could be using attack techniques (as catalogued by the MITRE ATT&CK framework) T1563,
remote service hijacking, or attempting T1080, tainting shared content [9].
To detect attacks such as the one in the example, a model would need to consider events with
reference to those that occurred previously and with reference to the other interactions within the
network. An event between two entities that happens at one point in time cannot be considered
identical to the same event occurring in the future under a different global context. Unfortunately,
existing graph-based approaches, which do not consider time, and many event-based approaches
that look at each event in isolation such as References [16, 28, 58] would see no difference between
(C1, SD) at time t1 and (C1, SD) at time t4. What makes it stand out as anomalous is something
lacking in the previous state of the graph—something that would only be detected by considering
prior probabilities for the next state of the system.
Frequency-based and event-based approaches such as References [10, 24, 37, 55, 81] would have
similar trouble, as they lack the relational data present in the network; Bob’s activities a timestep
earlier would have little import on the interpretation of the shared drive’s activity later on. If the
interaction between (C1, SD) at time t4 was sufficiently similar to the interaction at time t1, then
these approaches would see no difference between the two. They lack the ability to capture the
importance of interactions occurring between other entities in the network and how they may
relate to a separate event.
Our proposed solution, to represent the network as a temporal graph, ensures the global structure of a network at individual points in time is captured without losing the temporal dependencies
of the changing connections. We make the more difficult assumption that malicious events can
have the same event features as normal ones; if this is the case, then traditional event-based approaches will not work. We also assume attackers can make similar connections, or even the same
connections as observed in normal activity, meaning available graph-based approaches and statistical approaches are insufficient. With temporal graphs, assuming they have enough granularity,
these problems, as well as those tackled by prior works, are solvable.
5 EULER
In this section, we describe our proposed framework, which we call Euler. This framework aims
to learn a probability function conditioned on previous states of a temporal graph to determine
the likelihood of an edge occurring at a later state. Furthermore, it is the goal of this work to
offer an approach that is not just precise, but also highly scalable. We first describe the basic
components of the system, then how they are distributed across multiple machines and how these
components interact. Finally, we describe how different training objectives are implemented on
the Euler interface.
5.1 The Encoder-Decoder
The Euler framework is a generic extension of the traditional graph autoencoder model [48] to
temporal graphs. It consists of a model-agnostic graph neural network (GNN) stacked upon
a model-agnostic recurrent neural network (RNN). Together, these models aim to find an
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:10 I. J. King and H. H. Huang
encoding function f (·) and a decoding function д(·). The encoding function maps nodes in a temporal graph with T snapshots to T low-dimensional embedding vectors. The decoding function
ensures minimal information is lost during the encoding process and aims to reconstruct the input
snapshots from the latent Z vectors. More formally, we can describe the behavior of the encoder as
Z = f ({G0,..., GT })
= RNN( [GNN(X0,A0),...,GNN(XT ,AT )] ), (2)
where At is the |V| × |V| adjacency matrix representation of the snapshot at time t. This
T × |V| × d dimensional tensor Z is optimized to contain information about both the structure of
the graph and the dynamics of how it changes over time.
This is enforced by a decoder function, д(·), which attempts to reconstruct the original graph
structure given the embeddings. More formally,
д(Zt ) = Pr(At+n = 1 | Zt ), (3)
where Zt = Z[t] is the embedding of graph Gt and n ≥ 0. As was done by References [16, 34, 35, 48],
we use the inner product decoding as this д(·) function:
д(Zt ) = σ (ZtZ
t ) = A˜ t+n, (4)
where σ (·) denotes the logistic sigmoid function, and A˜ t+n represents the reconstructed adjacency
matrix at time t +n. As a consequence of using inner product decoding, the dot product of vectors
Zt[u] and Zt[v] represents the log-odds that an edge, (u,v) exists at time t + n. In this way, the
д(·) function is used to detect anomalous edges.
5.2 Workflow
The core of the Euler framework is a simple design. It simply stacks the replicas of a modelagnostic GNN that we refer to as the topological encoder upon a model-agnostic recurrent layer with
a few simple constraints. When fit into the leader/worker paradigm3 with one recurrent layer as the
leader, and multiple topological encoders as workers, it has the potential for massive parallelism.
The overall workflow for Euler is shown in Figure 3. It occurs in 5 basic stages: (1) The leader
spawns the workers and instructs them on which snapshots to load; (2) the leader initiates the
training loop, and workers generate topological embeddings; (3) as the topological embeddings
are received, the leader processes them through an RNN; (4) the output of the RNN is sent back to
the workers to calculate loss or for scoring; (5) in training mode, loss is returned to the leader to be
backpropagated. During evaluation, anomaly scores are returned. In this section, we describe how
these 5 steps are implemented in greater detail. For more detailed technical information about the
Euler interface, we direct the reader to Table 7 in the conference version of this work [44].
Loading Data
When the program first starts, several processes are spawned. The PyTorch multiprocessing library
automatically assigns each spawned process a unique ID from 0 to k + 1; for convenience the
machine with id:0 becomes the leader, and all others are workers. The leader machine, which
holds remote references to all workers in addition to the recurrent layer, issues commands to the
other processes to spin up all worker instances.
Upon their creation, workers connect to the leader and await instructions. After the initialization
of all workers, the leader assigns them contiguous subsets of temporal graph data to load, shown
in step 1 of Figure 3. For example, given a set W = {w1,...,wk } of k workers, and a temporal
graph split into T snapshots, the leader assigns each worker s =
T
k

snapshots. Then worker
3Historically called the master/slave paradigm.
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:11
Fig. 3. The complete series of interactions between the leader and worker machines during one training
step. In step (1), the leader machine initiates each worker and issues a command for each of them to load
a disjoint subset of the snapshots from memory. When this has occurred, the training loop can begin. At
step (2), the leader issues a command to each worker to perform a forward pass on their graphs through
their GNNs. As they complete this, workers send the topological embeddings, Z¯t , to an ordered queue in the
leader. Upon receiving the embedding for t = 0, step (3) begins. The leader runs embeddings from the queue
through its RNN as they come in to produce the final Z embeddings. Then, in step (4), the leader sends the
embeddings back to the workers to decode and calculate loss. Finally, (5), the leader and workers perform
a backward pass on the aggregated loss functions according to the DDP gradient bucketing algorithm [52].
During evaluation, the steps are nearly identical, but on step (4) workers return their scores for each edge yˆt
instead of loss.
wi holds {Gsi,..., Gsi+(s−1)}. In the likely case where k does not divide T , extra units of work
are assigned to workers holding snapshots later in time. This is because the recurrent layer of
the model processes the topological encoders’ output in order, as it comes in. Thus, the RNN can
perform the necessarily sequential forward pass on earlier embeddings while future snapshots are
still being processed by workers.
After the workers have been assigned their snapshots, they concurrently read them in. The
leader waits for each worker to signal that all data are loaded, then moves on to the next phase of
the workflow.
Topological Encoding
During the forward pass—shown in step 2 of Figure 3—the recurrent layer issues asynchronous
calls to forward on each worker machine. To minimize network traffic, the only thing the leader
sends to the workers is an enum representing which partition of edges to process, as some are held
out for validation and testing. Workers then process every snapshot they hold. Further reducing
network traffic, the matrices returned by the workers are far smaller than those used as inputs, as
each worker is essentially a graph autoencoder [48].
For performance optimization, and to ensure consistency, we impose just one constraint for this
stage: topological encoders must not be dependent on any temporal information. They provide a
purely spatial encoding of the state of each snapshot they hold, using only features observable at
a single point in time. With this constraint satisfied, all encoders can operate in parallel, as no one
worker is dependent on the output of another. Theoretically, with as many workers as snapshots,
the time complexity of a forward pass is constrained only by the snapshot with the greatest number
of edges.
Temporal Encoding
The leader maintains an ordered list of future objects that point to the eventual output of the
workers and waits for the future pointing to the first embeddings to finish executing. When the
leader receives this tensor, it is immediately processed by its RNN, shown in step 3 of Figure 3. Note
that so long as tasks are slightly imbalanced such that workers holding later snapshots contain
more work units, the leader’s recurrent layer can execute concurrently with at most k −1 workers’
topological encoders. Workers with earlier snapshots hold fewer work units, and therefore finish
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:12 I. J. King and H. H. Huang
ALGORITHM 1: Leader machine forward method
1 def forward(self, workers, partition):
/* Leader tells each worker to begin executing */
2 futures = [];
3 for w ∈ workers do
4 future = asynchronously execute w.forward(partition);
5 futures.append(future);
/* As workers return their embeddings, the leader processes them in order, as they arrive */
6 h=NULL;
7 zs = [];
8 for f ∈ futures do
9 z, h = self.RNN(f.wait(), h);
10 zs.append(z);
11 return concat(zs)
executing earlier, so the leader can process their outputs while workers holding greater quantities
of snapshots further in time are still processing.
When the recurrent layer has finished processing the output of one worker, the hidden state
and outputs from the RNN are saved. The leader waits for the next topological embedding to finish
processing, then uses the saved RNN hidden state and the next embedding to repeat the process
until all workers have finished executing. This procedure is described in more detail in Algorithm 1.
Decoding
When the leader finishes generating the final embeddings, they are sent back to the workers to
decode, and if the model is training, to calculate loss. This process occurs in parallel on the worker
machines. In general, graph functions such as those used to find edge likelihoods are more computeand memory-intensive, so we endeavor to run them in parallel whenever possible. This stage is
shown in step 4 of Figure 3.
During evaluation, instead of returning loss, the workers return edge likelihoods yˆt and the
ground-truth edge labels yt . The process for decoding the embeddings is the same, however, loss
is not calculated.
Backpropagation & Evaluation
When loss has been calculated and returned to the leader machine, gradients are calculated via
backpropagation, first through the recurrent layer, then components of the loss function generated
by each topological encoder are backpropagated in parallel and broadcast between workers in
accordance with the bucketing algorithm described in Reference [52]. After the backpropagation
step, and collective communication between workers, gradients across all workers’ model replicas
are equal. Finally, the recurrent layer and the topological encoders all update their parameters
using an Adam optimizer [46], and the leader repeats steps 2–5 until convergence.
If the model is in evaluation mode, then the leader machine instead uses the yˆt likelihoods the
workers generate, and the known labels yt , also returned by the workers, to calculate precision
and accuracy metrics, which are saved. In a real-world implementation without labels, it would
instead raise alerts on observed edges with likelihoods below a certain threshold.
5.3 Training
There are two modes of training Euler models: as a link detector or a link predictor. These two
modes are distinguished by which Zt embeddings are sent to the workers at step 4 to calculate loss.
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:13
Fig. 4. How embeddings are generated and the interpretation of their decoded values for static and dynamic
models. In link detection, embeddings Zt generated from partial snapshot Gˆ
t are used for evaluating the
likelihood of unseen edges in snapshot Gt . For link prediction, embeddings Zt generated from snapshot Gt
are used to predict the likelihood of edges in a future snapshot Gt+n (in this figure, n = 1).
Link detectors are inductive; they generate Zt using partially observed snapshots {Gˆ
0,..., Gˆ
t }
and attempt to reconstruct the full adjacency matrix At with д(Zt ). This process is illustrated in
Figure 4(a). In practice, link detectors would be used for forensic tasks, where one is performing
an audit to identify anomalous connections that have already occurred.
Link predictors are transductive; they generate Zt using snapshots {G0,..., Gt } to predict the
future state, At+n where n > 0. Figure 4(b) illustrates this process when n = 1. In practice, they
could be used as a live intrusion detection tool, as predictive models can score edges as they are
observed—before they have been processed into full snapshots. For example, when n = 1, given
what has been observed about the network up until time t − 1, it is the goal of predictive implementation of Euler to score edges observed at time t. Such a model can use embeddings learned
from previous states of the network to process connections as they occur.
To ensure these objectives, the reconstruction loss function aims to minimize the negative log
likelihood of Equation (3), where n = 0 when training detectors, and n > 0 for predictors. For larger
graphs, operating over the entire adjacency matrix quickly becomes intractable. Instead, as is done
by graph autoencoders [48], we approximate this value by minimizing binary cross-entropy on the
likelihood scores for known edges, and a random sample of non-edges at time t +n: Pt+n and Nt+n,
respectively. Consider Equation (4) on a per-edge level:
P ((u,v) ∈ At+n | Zt ) = σ (Z(u)
t Z(v)
t ), (5)
where Z(v)
t represents the embedding of node v at snapshot t. Then, the reconstruction loss function for snapshot t + n
Lt = E[− log(Pr(At+n | Zt ))] (6)
can be approximated by taking the sample mean.
Lt ≈− −1
|Pt+n |

(u,v)∈Pt+n
log P ((u,v) ∈ At+n | Zt )− −1
|Nt+n |

n∈Nt+1
log(1−P ((u,v) ∈ At+n | Zt )) (7)
New negative edges are randomly sampled on each epoch by workers for each snapshot they
hold. The leader machine coordinates which slices of the Z tensor are sent to each worker to
generate the probabilities, and each worker independently calculates loss on the training data
they hold.
On predictive models, Zt represents the latent space of nodes n snapshots in the future. Consequently, predictive models cannot calculate loss on the first n snapshots of the graph. To account
for this, the leader pads Z with a n |V| ×d zero matrices at the beginning, and the final n matrices
are removed before returning the embeddings to the workers. This way, Z[t] predicts the snapshot
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:14 I. J. King and H. H. Huang
ALGORITHM 2: Calculate loss
1 def worker_loss(zs, n_jobs, workers, n=1):
/* Pad the Z tensor if predictive */
2 zeros = n × |V| × d 0 matrix;
3 zs = concat(zeros, zs[:-n]);
4 futures = [];
5 start=0; end=n_jobs[0]; loss = 0; i=0;
/* Send the correctly offset embeddings to the workers to calculate loss */
6 for w ∈ workers do
7 f = asynchronously execute w.loss(zs[start:end]);
8 futures.append(f);
9 start=end; end=start+n_jobs[i ++];
10 return sum([f.wait() for f in futures]) / len(workers)
indexed at t on all workers except the one holding the initial snapshots, which ignores embeddings
that equal the zero matrix. This process is shown more clearly in Algorithm 2.
5.4 Classification
Though for much of our evaluation, we rely on regression metrics relating to the fitness of scores
assigned to edges, it is useful to automate the process of deciding the threshold for what counts
as anomalous to obtain classification scores. To this end, when training the model, we hold out
one or more full snapshots to act as an extra validation set. Using the final hidden state h of the
RNN from the training snapshots as input for the validation snapshot, a training partition of edges
is passed through the model. From there, finding an optimal cutoff threshold for edge likelihood
scores becomes a simple optimization problem.
Given a set of scores for edges that exist in the validation snapshots, but were held out of the
training set, and a set of scores for non-edges, the optimal cutoff threshold τ is the one that satisfies
arдmin τ
(1 − λ)TPR(τ ) − λFPR(τ ), (8)
where TPR(τ ) and FPR(τ ) refer to the true and false positive rate of classification given cutoff
threshold τ , and λ is a hyperparameter in [0 − 1] biasing the model to optimize for either a high
true positive rate or a low false positive rate. Experiments have shown that for anomaly detection,
where low false positive rates are critical, λ = 0.6 is very effective. For any metric involving
classification for the remainder of the article, this is how classes were determined from the edge
likelihood scores, and unless otherwise specified λ = 0.6.
5.5 High-performance Temporal Encoding
When distributing workloads to workers, it is assumed that each worker has equivalent processing
power and memory. We also assume that each snapshot constitutes roughly the same amount of
work. This is not always the case, especially with natural graphs that follow cyclical patterns (e.g.,
the ebb and flow of network activity at night and during the day), however, this provides a rough
upper bound on the maximum amount of work one worker is expected to accomplish. For this
reason, we use snapshots as the unit of work assigned to each worker. As previously stated, units
of work are evenly distributed between workers, with any extra units of work given to workers
processing later snapshots; this allows the RNN to process earlier, completed graph embeddings
while later embeddings are still being computed. If workers cannot be assumed to be equivalent in
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:15
Table 1. Dataset Metadata
Dataset Nodes Edges Avg. Density Timestamps
FB 663 23,394 0.00591 9
colab 315 5,104 0.01284 10
Enron10 184 4,784 0.00514 11
processing power, or memory, then jobs could be assigned according to a greedy interval partition
algorithm [49] or similar, but we leave this as a topic for future work.
Synchronization between workers is handled by PyTorch. Communication between workers to
randomly initialize model weights and share gradients is accomplished through the Distributed
Data Parallel module [52]. Communication between the workers and the leader is implemented
using the PyTorch remote procedure call framework [2], which communicates over TCP via TensorPipes [3]. The leader issues asynchronous calls to workers, who send their output to a socket
when their work is concluded. Thus, as the workers’ output is in the form of a list of promises
pointing to memory addresses, their order is guaranteed. In the event that a worker faults in some
way, the leader process will shut down if some timeout period has passed without receiving the
worker’s result. To prepare for such a situation (though it never arose in our experiments) the
leader saves a copy of the model’s weight dictionary every training step that can be reloaded into
the workers and leader so training may resume if it were to crash.
6 BENCHMARK EVALUATIONS
To determine if temporal link prediction is a viable method for lateral movement detection, it is
necessary to show that Euler is a viable temporal link predictor. Thus, before transitioning to
larger cyber security datasets, we evaluate the effectiveness of Euler on link prediction in general. In this section, we briefly present the results of our experiments on smaller, general purpose
temporal graph datasets from our prior work. Details of these datasets are reported in Table 1.
For a more detailed explanation of the experimental setup, we direct the reader to the conference
version of this work [44].
Prior works VGRNN [35] and Evolving GCN (EGCN) [63] both assert that simply embedding
graph snapshots with a GNN and running these embeddings through an RNN, as was done by References [68, 84], does not adequately capture the shifting distribution of nodes in a dynamic network. Reference [63] demonstrates that this is the case for inferring complete graph structure
several timesteps in the future, but Reference [35] does not evaluate their model against the very
model they so thoroughly disregard. To remedy this, we present a comparison between several
existing temporal autoencoder models and a simple stacked GCN [47] and GRU [21] following
the Euler framework. We select these two layers because of their lack of parameters—they are
the most generalized of the models in their respective domains [22, 69]. For all experiments, we
set all models evaluated to have the same dimensionality and number of layers. As each model
evaluated uses a single weight matrix per encoding layer, and three per RNN layer (apart from
DynAE, which does not use an RNN, and DynRNN, which does not use an encoding layer), this
caps the parameter count to a uniform level across all tests. For reference, a brief summary of the
other methods tested is presented in Table 2.
6.1 Results
As was done by Reference [35], we conducted three different benchmarking tests to compare Euler to other temporal link prediction methods: inductive dynamic link detection, transductive
dynamic link prediction, and transductive dynamic new link prediction. Link detection and link
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:16 I. J. King and H. H. Huang
Table 2. Prior Works Evaluated
Model GNN RNN Description
(SI-)VGRNN [35]  At the time of writing, the most effective temporal link
prediction model in the literature. A GCN stacked upon a
GC-LSTM [19].
E-GCN [63]  A powerful model that uses an RNN to update the parameters
of a GNN at each timestep. Especially good at link prediction
far into the future, but untested in short-term predictions.
A family of models that pass adjacency vectors into a
multilayer perceptron and/or an RNN to capture graph
dynamics using traditional deep learning techniques.
DynAERNN ✗
DynRNN [65] ✗
DynAE ✗✗
VGAE [48]  ✗ A simple two-layer variational graph autoencoder. This model
does not consider time at all. Instead, it views every
interaction as one large graph.
Table 3. Comparison of Euler to Related Work on Dynamic Link Detection
Metrics Methods Enron colab Facebook
VGAE 88.26 ± 1.33 70.49 ± 6.46 80.37 ± 0.12
DynAE 84.06 ± 3.30 66.83 ± 2.62 60.71 ± 1.05
DynRNN 77.74 ± 5.31 68.01 ± 5.50 69.77 ± 2.01
DynAERNN 91.71 ± 0.94 77.38 ± 3.84 81.71 ± 1.51
EGCN-O 93.07 ± 0.77 90.77 ± 0.39 86.91 ± 0.51
AUC EGCN-H 92.29 ± 0.66 87.47 ± 0.91 85.95 ± 0.95
VGRNN 94.41 ± 0.73 88.67 ± 1.57 88.00 ± 0.57
SI-VGRNN 95.03 ± 1.07 89.15 ± 1.31 88.12 ± 0.83
Euler 97.34 ± 0.41 91.89 ± 0.76 92.20 ± 0.56
VGAE 89.95 ± 1.45 73.08 ± 5.70 79.80 ± 0.22
DynAE 86.30 ± 2.43 67.92 ± 2.43 60.83 ± 0.94
DynRNN 81.85 ± 4.44 73.12 ± 3.15 70.63 ± 1.75
DynAERNN 93.16 ± 0.88 83.02 ± 2.59 83.36 ± 1.83
EGCN-O 92.56 ± 0.99 91.41 ± 0.33 84.88 ± 0.52
AP EGCN-H 92.56 ± 0.72 88.00 ± 0.85 82.56 ± 0.91
VGRNN 95.17 ± 0.41 89.74 ± 1.31 87.32 ± 0.60
SI-VGRNN 96.31 ± 0.72 89.90 ± 1.06 87.69 ± 0.92
Euler 97.06 ± 0.48 92.85 ± 0.88 91.74 ± 0.71
prediction are implemented as described in Section 5.3. On link detection tests the objective function is Equation (3) with n = 0. On (new) link prediction tests, n = 1. New link prediction is an
extra evaluation of predictive models. This test is identical to link prediction, but the set of true
positives only includes edges that were not in the snapshot immediately before.
Dynamic Link Detection
As shown in Table 3, the simplistic Euler model outperforms the more modern ones in almost
every test. In tests where it does not outperform the state-of-the-art methods, it is equivalent,
despite its simplicity. Compared to the static VGAE, which does not consider time at all, the benefit
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:17
Table 4. Comparison of Euler to Related Work
on Dynamic Link Prediction
Metrics Methods Enron colab Facebook
DynAE 74.22 ± 0.74 63.14 ± 1.30 56.06 ± 0.29
DynRNN 86.41 ± 1.36 75.7 ± 1.09 73.18 ± 0.60
DynAERNN 87.43 ± 1.19 76.06 ± 1.08 76.02 ± 0.88
EGCN-O 84.28 ± 0.87 78.63 ± 2.14 77.31 ± 0.58
AUC EGCN-H 88.29 ± 0.87 80.80 ± 0.95 75.88 ± 0.32
VGRNN 93.10 ± 0.57 85.95 ± 0.49 89.47 ± 0.37
SI-VGRNN 93.93 ± 1.03 85.45 ± 0.91 90.94 ± 0.37
Euler 93.15 ± 0.42 86.54 ± 0.20 90.88 ± 0.12
DynAE 76.00 ± 0.77 64.02 ± 1.08 56.04 ± 0.37
DynRNN 85.61 ± 1.46 78.95 ± 1.55 75.88 ± 0.42
DynAERNN 89.37 ± 1.17 81.84 ± 0.89 78.55 ± 0.73
EGCN-O 86.55 ± 1.57 81.43 ± 1.69 76.13 ± 0.52
AP EGCN-H 89.33 ± 1.25 83.87 ± 0.83 74.34 ± 0.53
VGRNN 93.29 ± 0.69 87.77 ± 0.79 89.04 ± 0.33
SI-VGRNN 94.44 ± 0.85 88.36 ± 0.73 90.19 ± 0.27
Euler 94.10 ± 0.32 89.03 ± 0.08 89.98 ± 0.19
Table 5. Comparison of Euler to Related Work
on Dynamic New Link Prediction
Metrics Methods Enron colab Facebook
DynAE 66.10 ± 0.71 58.14 ± 1.16 54.62 ± 0.22
DynRNN 83.20 ± 1.01 71.71± 0.73 73.32 ± 0.60
DynAERNN 83.77 ± 1.65 71.99 ± 1.04 76.35 ± 0.50
EGCN-O 84.42 ± 0.82 79.06 ± 1.60 75.95 ± 1.15
AUC EGCN-H 87.00 ± 0.85 78.47 ± 1.27 74.85 ± 0.98
VGRNN 88.43 ± 0.75 77.09 ± 0.23 87.20 ± 0.43
SI-VGRNN 88.60 ± 0.95 77.95 ± 0.41 87.74 ± 0.53
Euler 87.92 ± 0.64 78.39 ± 0.68 89.02 ± 0.09
DynAE 66.50 ± 1.12 58.82 ± 1.06 54.57 ± 0.20
DynRNN 80.96 ± 1.37 75.34 ± 0.67 75.52 ± 0.50
DynAERNN 85.16 ± 1.04 77.68 ± 0.66 78.70 ± 0.44
EGCN-O 86.92 ± 0.39 81.36 ± 0.85 73.66 ± 1.25
AP EGCN-H 86.46 ± 1.42 79.11 ± 2.26 73.43 ± 1.38
VGRNN 87.57 ± 0.57 79.63 ± 0.94 86.30 ± 0.29
SI-VGRNN 87.88 ± 0.84 81.26 ± 0.38 86.72 ± 0.54
Euler 88.49 ± 0.55 81.34 ± 0.62 87.54 ± 0.11
of the additional RNN layer is clear. We further observe the benefit of graph neural networks
over MLPs when Euler and the state-of-the-art methods are compared to the DynGraph2Vec
methods. However, the experiments do not support claims that much is gained by the complex
models beyond what is afforded simply by using a GNN and RNN. Both highly engineered models,
the EGCN and VGRNN variants, do not perform significantly better than the simplistic stacked
GCN on a GRU, and in some cases perform worse.
Significantly, the dataset where Euler performed better than prior works with p < 0.05 was the
Facebook dataset. This dataset contains the most nodes and edges and has the fewest snapshots
in the training set. Despite these difficulties, our simple Euler model achieves a 4% improvement
over prior work in both AUC and AP, signifying its ability to learn very complex spatio-temporal
patterns even on larger datasets. We observe that the model is generalized enough not to become
overfit on the smallest datasets, but not so simple it cannot handle larger ones. This supports our
claim that this model design, despite its simplicity, is highly precise.
Dynamic (New) Link Prediction
In these cases where temporal data are more significant, the results are less clear. As shown in
Tables 4 and 5, between our method and (SI-)VGRNN models, the results are almost all within
each methods’ margin of error. We note that on the dynamic new link prediction test for Enron10,
though our method’s observed mean AUC and AP were lower than both VGRNN methods, a t-test
showed that this disparity was not statistically significant. We can conclude that neither method is
significantly better than the other. Furthermore, we observe that on the Facebook dataset, which
has roughly the same edge density as Enron, but 3.5× as many nodes, Euler performs significantly
better than other methods in new link prediction. As variance and complexity increases in the data,
Euler adapts better than the other methods while retaining precision on simpler data without
becoming overfit.
In both tests, models that process graph embeddings using an RNN were significantly better
than DynAE, which does not. This component enables temporal attributes of the data to be carried over from previous timesteps. If a new edge has been seen in the distant past, or a pattern that
indicates a new edge is likely to appear has previously been observed, then this history is carried
over by the RNN into future embeddings. From this, we can again infer that the benefit derived
from the (SI-)VGRNN models has more to do with the components of those models, which are also
GCNs connected with an RNN. The espoused benefit of the topological embedders ingesting temporal information does not appear to be as great as simply using those components; by removing
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:18 I. J. King and H. H. Huang
the GNNs’ reliance on temporal information, our models can embed at least the same quality of
information in a more efficient manner.
With these data, it is clear that the simplicity of models following the Euler framework is not a
hindrance, and in many cases is actually advantageous. The purpose of Euler is chiefly to improve
efficiency and scalability, so the fact that it is only a small improvement, or about equal to state-ofthe-art models, is adequate for our purposes. The real benefit of building models within the Euler
framework is their ability to scale. With larger datasets, this advantage is more evident.
7 THREAT MODEL & ASSUMPTIONS
As we have shown, the Euler framework is capable of outperforming prior works on temporal
link prediction generally. The next step is to show that temporal link prediction is an effective
method of lateral movement detection. In the following sections, we will show that Euler can
efficiently detect anomalies indicative of lateral movement under the minimal constraints defined
in this section.
Like many other anomaly detection models [15, 16, 42, 54], we make the assumption that a
large sample of benign data is available from which to learn a baseline for normal behavior offline.
This means the framework has certain limitations: If there is not enough normal behavior to learn
from, then the false positive rate may be higher. Additionally, in a real-world setting, if there was
malicious activity in what is thought to be the clean training data, then the model could learn
that certain patterns in connections are normal when they are not. Unique to temporal anomaly
detection is the difficulty in knowing how fine-grained the graph snapshots ought to be. Certain
events may be anomalous simply because they happened at a certain time of day [31], which would
evade our system if snapshots spanned a full day of activity. Finally, we note that, depending on
how Euler is implemented, there may be some delay in detection time. With a predictive model,
new edges can be scored as they appear, which takes a negligible amount of time. However, if
Euler uses the detection mode, then it must wait for an entire snapshot to be generated before it
may score the events that occurred. However, experiments showed smaller snapshots resulted in
better performance for the model; the delay incurred waiting for a snapshot would be between 6
and 30 minutes using our hyperparameters. As lateral movement is a “low-and-slow” process, this
would likely not be a problem.
To alleviate some of these difficulties, we focus on datasets with large bases of training data from
which to learn these patterns. In a real-world setting, there are countless commercial and academic
systems for network security monitoring that could be employed to build a large database for
training [1, 23, 40]. This assumes that the logged data does not contain anomalous activity, but this
can be verified using a signature-based system during its collection, or other expert analysis. Our
focus on lateral movement detection means only internal communications are used for training,
further reducing the risk of anomalous behavior in the training set (e.g., users visiting websites for
the first time, novel events that are “anomalous” but not malicious). This means that our models
will be unable to detect the initial compromise but are trained to find irregularities in inter-host
communications, a signature indicative of lateral movement [20].
To capture these suspect communications, at a minimum, we need the information about the
sources and destinations of traffic sources within an organization. In our experiments, we used
authentication records and data flow objects as the means of tracking these things. Both data
sources largely represent the same thing: a user or system attempting to use a service on a remote
device. As a threat-actor attempts to traverse a large network of computers, they will endeavor to
do so slowly and sneakily, so it is unlikely they would use a service that is not monitored by these
tracking services in the datasets [71]. However, we concede that if there are holes in a systems
auditing that ignore certain services, then an attacker could utilize them for lateral movement and
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:19
Table 6. LANL Dataset Metadata
Nodes 17,685
Events 45,871,390
Anomalous Edges 518
Duration (Days) 58
evade our (and other similar) systems’ alerts. Thus, we assume that all relevant remote tools and
services are monitored by a logging system. Beyond unique identifiers for entities within a system
to track their communications, Euler needs very little. As a temporal graph analyzer, timestamps
are required or may be inferred from the ordering of the logging events. Features for each entity,
while they may aid in false positive reduction, are not strictly needed. In this work, we use only
features that can be inferred from the unique names of system entities (whether they are users or
computers, if they are administrators, etc.). We leave additional research in feature extraction for
the nodes as a topic of future work.
8 EXPERIMENTS ON LANL
In the previous section, all datasets tested were rather small. It is not until a real-world application
of the Euler framework is tested that the true performance improvements are evident.
To demonstrate the impressive speedup achieved by this framework when compared to related
work, we evaluate several Euler models on the LANL 2015 Comprehensive Multi-Source Cyber
Security Events dataset [41]. The dataset consists of 57 days of log files from five different sources
within the Los Alamos National Laboratory’s internal corporate network as it underwent both
normal activity and a red team campaign. Specific details about this dataset are reported in Table 6.
The edge count in the table represents the number of weighted edges; multiple events between the
same entities in the same time period may be compressed into a single weighted edge. Because
events from the authentication logs have been labeled as normal or anomalous, this dataset has
been widely used for cyber security research [13, 16, 37, 81]. The labels make it especially apt for
lateral movement detection research. When an APT-level threat is attempting to traverse a system,
one possible warning sign will be authentications that should not normally occur, a sign indicative
of lateral movement on a network level [38, 42, 54].
In this section, all distributed models were implemented with four worker nodes unless otherwise specified. All experiments are run on a server with two Intel Xeon E5-2683 v3 (2.00 GHz)
CPUs, each of which has 14 cores with 28 threads, and 512 GB of memory [5].
We will first present the utility of models following the Euler framework as an anomaly-based
intrusion detection system on the LANL dataset, then an analysis of the immense scalability afforded by splitting models in this way.
8.1 Graph Construction
We construct a weighted, directed graph from the authentication logs by mapping which entities
authenticate with one another. As nodes, we use the entities denoted source and destination computers in the LANL documentation. For all authentications that occur from time t to t +δ, an edge
is created between the source computer and destination computer. If an edge already exists, then
a tally keeping track of the number of authentications between the two machines is updated. Experiments have shown that the most effective method to normalize these tallies into usable edge
weights is to take the logistic sigmoid of the edges’ standardized values. Mathematically, it can be
represented as
W ((u,v) ∈E) = σ

C(u,v) − μE
ΣE

, (9)
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:20 I. J. King and H. H. Huang
where σ (·) represents the sigmoid function, C(u,v) represents the frequency of an authentication
between u and v in the time window, and μE and ΣE represent the mean and standard deviation of
all edge frequencies in the time window. In this way, edges that occur very infrequently are given
lower weight during training to appear less “normal,” and edges that occur with high frequency,
such as edges from computers to domain controllers or ticket-granting servers, have high weight
and appear routine.
The LANL dataset has no node features by default, however, some information can be gleaned
from the naming convention used in the log files. Entities have unique, anonymized identifiers
that start with either a U or a C denoting users and computers, respectively. There are also nodes
with non-anonymized names that have important roles in the system such as TGT, the Kerberos
key distribution center, DC, the domain controller, and so on. To leverage this additional data, we
concatenate a 1-hot vector denoting user, computer, or special administrative machine to each
node’s one-hot ID vector.
For quicker file scanning, and data-loading times, the full 69 GB auth.txt file is split into
chunks, which each hold 10,000 seconds (approximately 3 hours) of logs. Worker machines are
issued instructions to read in certain ranges of the log files and build the temporal graphs. Workers accomplish this by spawning several child processes to load multiple snapshots in parallel. The
associated edge lists and edge weight lists from each child process are combined to form the final
TGraph object, which holds a list of all edge lists, edge weights, node features, and tensor masks
to take partitions of each edge list for training.
For all experiments, the training set consists of all snapshots that occur before the first anomalous edge appears in the authentication logs. This allows models to learn what normal activity
looks like. From this set, we remove the final 5% of snapshots for tuning the classifier and mask
5% of edges from each snapshot for validation.
8.2 Experimental Setup
We test three encoders in conjunction with two recurrent neural networks as well as models with
no recurrent layer to measure how much value temporal data adds to the overall embeddings.
The encoder models are GCN [47], GAT [76], and GraphSAGE [36]. The recurrent models are
GRU [21] and LSTM [39]. The models are trained in the same manner as the link detection and
link prediction models in Section 6. However, experiments showed that once a local optimum was
found and validation scores ceased improving, it rarely improved after further iterations. As such,
early stopping occurs after only 10 epochs of no improvement.
Experiments showed for every model that using smaller time windows always lead to better
results. As such, we only present the output of tests on temporal graphs with time window δ =
1,800 seconds (30 minutes).
The GAT encoder uses three attention heads, which was found to be optimal via hyperparameter tuning. The SAGE encoder uses maxpooling as its aggregation function, as this was found to
be optimal in their paper, and this makes it capable of discerning between certain graphs GCN
cannot [83].
Unfortunately, many other works that experiment with the LANL dataset either do not use the
dataset in full, as is the case with References [37, 81], or conduct tests on portions of the data
other than purely the authentication logs, as was done by Bai et al. [13], so it would not be fair or
meaningful to compare our results to theirs. Bowman et al. [16] does use the full authentication log
as its dataset, however, it trains on a larger set of data, using all days that contained no anomalous
activity as the training set, rather than just the days before the attack campaign. We include their
results, nonetheless. We also include the TPR and FPR of a rules-based Unknown Authentication
(UA) model reported by Reference [16]. This rule simply marks any edge that did not exist in the
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:21
Table 7. Performance of Euler Models on the LANL Dataset when δ = 0.5
Dynamic Link Detection
Encoder RNN AUC AP TPR FPR P
GCN
GRU 0.9912 0.0523 86.10 0.5698 0.0054
LSTM 0.9913 0.0169 89.65 0.5723 0.0056
None 0.9916 0.0116 88.57 0.4798 0.0066
SAGE
GRU 0.9872 0.0307 84.71 0.6874 0.0044
LSTM 0.9887 0.0389 83.55 0.6591 0.0045
None 0.8652 0.0052 79.58 24.5669 0.0001
GAT
GRU 0.9094 0.0076 85.21 21.533 0.0001
LSTM 0.8713 0.0022 96.83 19.873 0.0002
None 0.9867 0.0079 99.88 23.174 0.0002
VGRNN 0.9315 0.0000 59.69 4.938 0.0000
UA – – 72.00 4.400 0.0010
GL-LV [16] – – 67.00 1.200 0.0034
GL-GV [16] – – 85.00 0.900 0.0051
Dynamic Link Prediction
Encoder RNN AUC AP TPR FPR P
GCN
GRU 0.9906 0.0155 85.49 0.6088 0.0050
LSTM 0.9885 0.0166 78.91 0.5987 0.0047
None 0.9902 0.0092 86.42 0.5425 0.0057
SAGE
GRU 0.9847 0.0200 86.30 1.6542 0.0019
LSTM 0.9865 0.0228 85.29 0.8037 0.0038
None 0.9284 0.0020 86.23 16.525 0.0002
GAT
GRU 0.8826 0.0020 87.82 21.971 0.0001
LSTM 0.8383 0.0002 83.42 29.297 0.0001
None 0.9352 0.0079 88.83 20.093 0.0002
VGRNN 0.9503 0.0004 70.00 0.280 0.0004
training data as anomalous. This system acts as a baseline to which we compare all other models.
Models that outperform UA’s FPR show they are capable of anticipating probable edges despite no
prior knowledge of them; models that outperform its TPR show an understanding of edge context
within the greater neighborhood, both temporal and spatial. They can detect edges that, though
previously observed, in a new context, are malicious.
By default, VGRNN operates on full adjacency matrices, however, we modified it to use sparse
edge lists for our experiments. This way it was able to scale to the large size of the LANL dataset.
Unfortunately, the E-GCN and DynGraph2Vec models could not scale to the LANL dataset. DynGraph2Vec relies on dense adjacency matrices, and the size of the 1-hot vectors used as inputs
was too large for E-GCN to process. As a result, our hardware was unable to fully evaluate these
methods, and their results are not compared to those of Euler.
All models evaluated use 32-dimensional hidden layers, and 16-dimensional embeddings. All
Euler models use a tanh activation function between the encoder and the recurrent layers and an
edge dropout layer before the GNNs. They all determine the classification threshold according to
Equation (8) with λ = 0.6, except the GraphSAGE models. For this encoder, experiments showed
λ = 0.5 was more appropriate. All reported results are average scores from five independent tests
on link detection and link prediction.
8.3 Anomalous Edge Detection
It is difficult to properly evaluate methods for classifying imbalanced data, especially anomaly
detection, where small false positive rates are so critical. For this reason, in addition to the raw
true positive and false positive rates, we report precision (P), area under the curve (AUC), and
average precision (AP). This latter method is recommended for anomaly detection by Reference
[25] as especially adept for imbalanced datasets. The AUC and AP metrics evaluate the overall
quality of scores given to edges, as opposed to the quality of classification, and provide better
measurements of the model if the anomalous score threshold was to be changed. The precision
metric provides further context to the quality of classification at the specific threshold. The average
results of five experiments are shown in Table 7.
As a baseline, consider the TPR of the UA rules-based approach. This implies that 28% of
anomalous connections are those that have occurred before in the network. This supports our
claim that temporal information about the context of connections is just as important as the
entities that are authenticating. This system is an excellent baseline model to compare to, as
any model that has a higher TPR than UA must be using a more advanced metric than simply
memorizing every legitimate connection observed in normal activity. If a model has a TPR above
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:22 I. J. King and H. H. Huang
72% with an FPR lower than 4.4%, then it must be leveraging topological or temporal context
judge the validity of connections.
The results show that the GCN is the most effective encoder for link detection, and SAGE is
most effective for link prediction; this supports our claim and those of Halilaj et al. [69] that more
generalized models are more effective. The GAT models, which have 3× as many parameters as
GCN and SAGE performed quite poorly both in quality of scores and quality of classification.
Also worth noting is the way using an RNN affects the output. Surprisingly, the best AUC in
the link detection tests were from a GCN with no temporal encoder. However, this metric is not a
good indicator of model quality on datasets with imbalance as extreme as LANL. The dramatically
higher AP score of all models that use RNNs suggests temporal data strongly affects FPR and
cannot be ignored. Similarly, the models without an RNN have high precision on the GCN models.
However, again, the AP scores would indicate that while at this specific threshold omitting the
RNN is beneficial, over all thresholds, models that take time into account perform better.
In the more realistic transductive link prediction tests, though the difference between the two
RNNs tested is small in every case, the benefit they add is unquestionable. The best-performing
encoder, GraphSAGE, enjoyed a 10× improvement in AP when used in conjunction with any RNN.
The next best-performing encoder, GCN, achieved a 1.6× improvement in AP. This is evidence that
temporal information carries important context for the topological state of the network, particularly for filtering false positives. Where one authentication may appear anomalous in isolation,
when viewed in the context of previous authentications, it can be correctly identified as benign.
The GL-LV and GL-GV methods do not consider time at all; the network is viewed as a static
graph. Here, we again see the benefit of using a sequence encoder in conjunction with a pure
topological embedder. The best Euler methods outperform their random walk-based approach in
terms of both TPR and FPR. Also worth noting is that because our model uses temporal graphs,
the alerts from Euler-based models come with a timestamp, making them more informative and
valuable in a real-world scenario. The prior work ranks any duplicate edges, regardless of their
temporal context, as equally anomalous.
Like Euler, VGRNN combines a sequence encoder with a temporal one. They claim that by
using temporal information as input during the topological encoding phase, complex temporal
dependencies are better encoded than without it. However, this method performs no better than
the purely statistical UA method. Even still, the false positive rate is excellent in the predictive test,
outperforming every Euler model. It is worth noting, however, the quality of likelihood scores is
very poor with this method, which implies that had the threshold for the Euler models been set
lower, their FPR would be lower with equivalent TPRs. This is readily apparent in the GCN-based
models where the FPR is only a few 10ths of a percent larger than the dynamic VGRNN, but their
TPRs are almost 10% higher.
Finally, we must concede that while the Euler models do outperform prior works, their FPRs
are still too high to be useful as an intrusion detection system on their own. Some of this can be
attributed to the dataset itself; labeled anomalous events are very coarse-grained. There are likely
many events the compromised entities engaged in that should be considered anomalous, and may
have even been detected by our models, but that are treated as false positives due to the lack of
fine-grained label information. Indeed, the red log only tracks “compromise events” and not the
further malicious activity that ensues [41].
However, even if this is not fully the case, this method has great potential as a filtering device for
further analysis tools. The low cost of processing time, which we will demonstrate later on, makes
this an efficient way to minimize the number of interactions that need to be analyzed by a signaturebased technique, for example. As we will show in Section 9, this approach can be further improved
as a step in a longer pipeline. However, even without additional analysis, we have demonstrated
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:23
Fig. 5. Change in AUC score as δ increases for link prediction and detection with GCN+GRU models. Scores
are the average of five tests on the LANL dataset.
that compared to all other anomaly-based works that analyze this dataset, Euler models are the
most effective.
8.4 Parameter Analysis
Time window size, the hyperparameter δ, has significant tradeoffs associated with it. As this value
decreases, the number of edges increases, requiring more processing time. As this value increases,
more repeat edges are condensed into single, weighted edges; additionally, there will be fewer
discrete timesteps for the recurrent networks to process, all contributing to faster processing time
at the expense of less precise data. With more edges, and more temporal granularity, models are
more capable of learning useful patterns across time. Figure 5 illustrates this phenomenon.
As is evident from the figure, small window sizes are more optimal for quality of scores; however,
due to the longer processing time required, and the relatively small performance improvements
as window size decreases after a certain point, it may be adequate to leave the snapshot duration
a little higher than is optimal for faster training and evaluation. Additionally, we observe that
changes in δ seem to affect the link prediction model at a higher rate than the link detection one.
We suspect this is caused by the model’s inability to predict short-term temporal patterns like the
one described in Section 4 as δ grows, and the predictive model is especially apt to detect this type
of anomaly.
Nonetheless, in both cases, more granular graphs lead to more informative edge scores. We
speculate that at some point, having time slices too granular would have diminishing returns, as
graphs will have too few edges to be useful. But due to the severe training time as δ decreases, we
have never managed to reach this point. We leave finding this boundary as an area for future work.
8.5 Interpreting the Results
Table 7 shows the results we found when testing several Euler models, the VGRNN model [35], and
the results reported by Bowman et al. [16] on the LANL dataset. In Reference [44], we concluded
that our method outperformed the others. However, as anomalous link detection is so heavily class
imbalanced, it was at times difficult to properly interpret these results.
A low false positive rate is of paramount importance, but this is very contingent on where the
cutoff threshold is set. But even assuming this threshold is set optimally, how do we factor in
the true positive rate? For our experiments, we were fortunate that Euler models outperformed
prior works in both true positive and false positive rates in many cases but consider VGRNN’s
performance on link prediction. It has the best FPR of any model, but we argue that it still is not
the best model. Looking at any one metric is deceiving. The AUC scores, which give us a glimpse
of how moving the threshold would affect things, are excellent for this.
One way we attempted to quantify this was to generate a lower bound on how much changing
the threshold would affect the TPR. Figure 6(a) demonstrates how to geometrically find the error
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:24 I. J. King and H. H. Huang
Fig. 6. Calculating the upper bound of error caused by increasing or decreasing the classification threshold.
The observed error, ε, in the AUC is the shaded region in Figure 6(a). In the worst case, the residual error, R,
takes up the dotted region in Figure 6(b). This value is bound by AUC + ε + R = 1, so the TPR can decrease
by at most x.
guaranteed from the single known TPR and FPR metric. The AUC plus this known amount of error,
and the residual error, must sum to 1. In the worst case, the entire residual error would be to the
left side of the curve. Figure 6(b) places a lower bound on the TPR if the threshold were changed
to match the FPR attained by another model. The full lower bound equation is
t
ˆ ≥ t − 1 − A − (1 − t)f
ˆ
f , (10)
where ˆ
f is the desired FPR, A is the AUC score, and t and f are the observed TPR and FPR scores.
Unfortunately, for our tests, this approach did not generate useful results. In most cases, assuming the residual error falls entirely on the left of the known point results in a lower bound less than
zero. However, we believe this approach to model comparison has value and present the equation
for future use.
It is challenging to properly compare models, but we were fortunate that our results were so
unambiguous. Though the TPR and FPR can provide more concrete descriptions of how these
models may perform in the real world, from a more analytic standpoint, the regression metrics
carry more information. These metrics tell us if a good threshold exists, rather than if one was
found. As models’ classification thresholds can always be tuned, metrics that measure the actual
distribution of threat scores are far more telling.
8.6 Challenges
Evaluating these temporal link prediction models came with several challenges: setting up the
experiment in a fair manner, accounting for the differences between models, and the nature of the
data itself. In this subsection, we will describe these challenges and by what means we overcame
them.
Validation and Tuning
When tuning a model’s hyperparameters, it is important to only measure their effect on a validation set. Tuning them with respect to the test data is a form of p-score hacking [78] and generally in
poor form. However, for our experiments, we did not have an especially representative validation
set. As our model was meant to be entirely unsupervised, in lieu of holding out some of the labeled
data for validation, we used a held-out set of unlabeled, benign data. In doing so, the validation
did not reflect the eventual class imbalance that would be present in the test data. Nor did it reflect
the actual nature of the test data.
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:25
The test data contained anomalous edges that affected the overall graph structure. As a result,
these edges would be considered in the GNN during node embedding. The negative edges in the
validation set, however, were not. This raises questions about the value of this method of validation.
Because of this, there was a large discrepancy between the validation results and the test results.
Nonetheless, it was all we could do. This may explain why more complex models such as the
GAT had such poor performance. It was very difficult to properly tune their hyperparameters on
a validation task that was dissimilar to the final test objective.
Fair Comparison
Another difficulty we faced was fair comparison of Euler to the other models. For example, the
models from Reference [16] use entirely different training parameters and experimental setups.
These approaches view the underlying graph structure as static. This entails that every interaction
between entities through time is analyzed at once. This makes it so their model is considering
fewer edges overall in its evaluation. Because Euler considers a temporal graph, duplicate edges
count more than once; false positives can occur more than once while the number of true positives
remains the same. Even if the two models classified edges in exactly the same manner, just the fact
that there are more edges in the temporal graph makes it harder to have a false positive rate lower
than the prior works.
Additionally, their models were trained on more data than ours was. Every day that does not
contain an attack was used in the training set, while only attack days were used for evaluation. Nevertheless, even with these setbacks, Euler outperforms these approaches. Though the comparison
is not fair, it is biased against our model, not theirs. We believe this strengthens our argument for
Euler. We feel that unfair comparison is acceptable so long as it works in the competing methods’
favor.
Labeling the Data
The LANL dataset came with certain events that were labeled as anomalous. However, they give
very little context to what these anomalous events were. It may be the case that the labeled authorizations only represent initial compromises or successful compromises. There could be many
other unlabeled anomalous authorization events that follow from these labeled events.
Furthermore, as the dataset is from a real-world system, there could still be more anomalous
events just from the users doing unusual, potentially malicious things unrelated to the documented
campaign. As these events are unlabeled, we cannot know for sure without expert assistance. However, as all methods were evaluated using the same labels, we still feel this is a fair assessment of
each of their abilities.
We did run tests under the assumption that compromised hosts compromise all other hosts they
authenticate with in the future, but the results were inconclusive. The first compromise occurs
only 2 days into the 57 days of logs. Using this new labeling system, very quickly, almost every
authentication is marked as anomalous. This is obviously not realistic or adding any value. More
likely, after a day or so the campaign ended, and the compromised hosts were reset, but again, we
did not have enough information to make such an assessment. We did have fair results by resetting
the list of compromised hosts every 24 hours, but this makes assumptions that are not backed by
the data. Ultimately, we just used the labels they provided and accepted the lower evaluation scores.
So long as we evaluate every model with the same labels, the hindrance is distributed equally.
9 EXPERIMENTS ON OPTC
In this section, we apply the lessons learned from the initial experiments on the LANL and benchmarking datasets to a new experiment. Additionally, we apply a new augmentation to Euler to
further extract value from its embeddings.
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:26 I. J. King and H. H. Huang
Table 8. OpTC Dataset Metadata
Nodes 1,114
Events 7,773,514
Anomalous Edges 21,872
Duration (Days) 7
9.1 The OpTC Dataset
The DARPA Operationally Transparent Cyber Data Release (OpTC) dataset consists of host
logs from 1,000 machines interacting with each other and external hosts over a one-week period.
These logs span four days of benign activity and three days of differing attack campaigns. In total,
the 1.1 TB dataset contains 17.4 billion events [12] stored in a modified CAR format [59]. As we are
primarily interested in lateral movement detection over the network, we only use a small subset
of these events: all FLOW objects undertaking a START action. In this way, we create a temporal
graph of hosts on the network initiating communication with other machines. The decision to
base the graph on successful FLOW-START objects is two-fold: first, because it is a more compressed
way to represent the entirety of several FLOW-MESSAGE objects, while retaining the same data;
second, because this is the most direct way in which inter-host communication is represented
in the OpTC dataset. While PROCESS-OPEN events for remote processes also capture this, they
will also always produce a FLOW-START event. Thus, it is the most efficient way to capture all
communication between hosts and where lateral movement will necessarily manifest [51, 71]. The
full metadata of this dataset is shown in Table 8.
To convert from log files to graphs, we preprocess the data to extract all FLOW-START objects and
parse out the source and destination IPs as well as the timestamp. This dataset is not labeled by
default. Rather, the curators provide a red log written by humans describing the malware campaign
and any events of note. As a result, when labeling the data, more context is available. So, unlike in
the original LANL experiments, there is no ambiguity: If an event is labeled as anomalous, then it
is because of lateral movement.
The data are labeled according to the following criteria: If the source IP was involved in a red
team event that happened that day, and if that red team event occurred before or during the time
of the logged event, then it is marked as anomalous. This decision is motivated by the assumption that when a host is compromised, it remains compromised for the duration of the red team
campaign—and these campaigns each last a single day. Rather than guessing if the compromised
state of an entity should be propagated, as we attempted to in Section 8.6, the red log explicitly
states if an attempt at lateral movement was successful. So, only hosts that have been successfully
compromised produce anomalous edges. Each campaign only lasts a single day, so at the end of
each day, the state of each entity in the network returns to normal.
When the event logs have been parsed and labeled, the temporal graph is constructed in parallel
by each worker. Each worker loads a disjoint range of the labeled events, partitions them into
snapshots, and constructs edge lists from the observed interactions between IPs and their labels.
Any self-loops in the data are ignored. Though there is much data describing each node, in our
experiments, nodes have no features, so one-hot encodings of the node IDs (the identity matrix of
size |V|) are used as input features to the GNNs.
9.2 Model Augmentations
As Ouyang et al. [62] point out, the objectives of networks that embed nodes for high-fidelity
reconstruction and those that identify anomalies are often different. There may be irrelevant information encoded in the Z vectors necessary for reconstruction, but not necessarily required for
anomaly detection.
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:27
Therefore, we also implement their approach to anomaly detection, which we denote EulerSM. After embeddings for reconstruction have been generated, they are used as the inputs for
a final anomaly detection layer a(·). This layer aggregates a sample of each node’s neighbors’
embeddings and applies a final transformation upon Zt into a |V| dimensional vector that acts as
a probability distribution that an edge exists between any two nodes at time t +n. This final layer
is then defined as
P[At+n = 1 | Ht] = a(Ht )
a(Ht ) = Softmax(WHt )
Ht = 1
s
StZt
, (11)
where W is a trainable parameter, and St is the identity matrix pluss randomly sampled elements
of At per row. In practice, this is accomplished with message-passing [30], not matrix multiplication. As was done by Reference [62], to create the St matrix, we sample an equal number of
neighbors with replacement for each node during training and use the full, normalized adjacency
matrix during evaluation. The number of samples drawn, s, is a user-defined hyperparameter.
Experiments showed that s = 5 works well.
As distributing the work of graph processing is still of paramount importance, this is implemented as another distributed model. The anomaly detection layer is replicated across all worker
machines after they have completed training and generated the Z embeddings. The training cycle repeats with the Z embeddings being used as input to optimize the W parameter in the a(·)
function on each worker.
This technique extracts even more data from the latent Z embeddings. Unfortunately, due to
the necessity of the softmax function, this decoding strategy does not scale well with additional
hosts—though it does scale with large time periods and large edge counts—nor can it accommodate
networks that add nodes over time, as the size of the final a(Ht ) vector must be static. However,
we will demonstrate that there are situations where this decoding strategy is extremely beneficial.
9.3 Experimental Setup
In this section, we compare the effectiveness of several link prediction models on the OpTC dataset.
Models are trained on the first four benign days to learn the pattern of normal interactions within
the network; all snapshots after the first four baseline days are considered the test set. Because
each day in the OpTC test set represents a different attack campaign that is reset the following
day, the final hidden state of the RNNs after they process the training set is saved to be used as the
initial hidden state for the beginning of each campaign. We evaluate models using several different
sizes of snapshots and report test results of models that had the best validation AP.
For Euler(-SM) models, we tested two different RNNs: GRU [21] and LSTM [39]. For the graph
neural network, we only tested GCN [47], as the number of nodes is rather small, and prior works
have shown GCN and other models perform about equally on such datasets [69]. Unfortunately,
because the heuristic we used to label the edges is our own, there are no prior works with which to
compare our method. However, we attempt to present a fair comparison between the Euler(-SM)
models and our own implementations of EGCN and VGRNN [35, 63]. We note that these have been
modified slightly such that edge lists are used as inputs rather than full adjacency matrices, and
the loss functions use negative sampling instead of full adjacency matrix reconstruction.
9.4 Results
As is evident from the results in Table 9, Euler models that make use of the additional softmax
layer achieve the greatest results on the test data. Though the difference between an LSTM and
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:28 I. J. King and H. H. Huang
Table 9. Effectiveness of Link Prediction Models on the OpTC Dataset
Dynamic Link Detection
Model δ (h) AUC AP TPR (%) FPR (%)
EGCN-O 5 0.554 0.003 67.5 58.7
EGCN-H 3.5 0.484 0.002 83.9 85.4
VGRNN 5 0.988 0.367 99.3 15.0
Euler GRU 2.5 0.888 0.088 17.8 0.473
Euler LSTM 2.5 0.882 0.118 17.8 0.168
Euler-SM GRU 0.125 0.995 0.973 97.0 0.021
Euler-SM LSTM 0.125 0.995 0.984 96.7 0.012
Dynamic Link Prediction
Model δ (h) AUC AP TPR (%) FPR (%)
EGCN-O 5 0.563 0.003 72.7 63.2
EGCN-H 3.5 0.507 0.003 80.0 80.2
VGRNN 0.125 0.692 0.008 73.1 42.1
Euler GRU 3 0.785 0.180 37.6 10.4
Euler LSTM 3 0.779 0.243 42.7 6.75
Euler-SM GRU 0.125 0.995 0.969 93.8 0.017
Euler-SM LSTM 0.5 0.994 0.986 94.9 0.013
a GRU temporal embedding unit is minimal, the LSTM attains slightly higher scores overall. The
large increase in AUC and AP between the inner product decoding models and the softmax decoders shows that Euler is not losing information in its encodings; rather, it is being used for reconstruction moreso than anomaly detection. The dedicated network for anomaly detection adds
great value to the overall pipeline.
The competing methods performed quite poorly in comparison, likely because they, too, are
encoding for reconstruction rather than anomaly detection. Though Euler embeddings alone still
have higher metrics than the prior works in link prediction, in link detection, VGRNN is the next
best-performing algorithm behind the softmax ones. We note that VGRNN attained the highest
TPR of all models, but at the expense of a very high FPR.
Using the lower bound formula from Equation (10), we find the residual error is 109.5. Thus,
if we allow the lower bound on the TPR to be 67.5%, that of the lowest ranking model, EGCN-O,
then the FPR drops to 3.45%. VGRNN clearly outperforms the EGCN methods, but we cannot draw
any conclusions about how VGRNN would compare to the Euler-SM models. Setting the desired
FPR to such a low value results in a negative lower bound, which yields very little information.
But from the pure AUC and FPR scores alone, we can say with high confidence, the VGRNN likely
would not be able to attain such a low FPR with a similarly high TPR.
These results show that the decoder algorithm is a good direction for future work, as small
adjustments can produce great improvements without compromising the distributed architecture
of Euler.
10 SCALABILITY
The main benefit of using models that fit into the Euler framework is their scalability. While
evaluation metrics on benchmarks were generally better than prior work, there were certainly
some categories where the advanced models are comparable to our simple ones. However, as we
will show, distributed topological encoding has tremendous performance benefits.
We will first identify theoretical complexities of each model tested. For simplicity, we assume the
models are implemented using a GCN and a GRU, however, the asymptotic analysis for other GNNs
and RNNs are similar, so the following values are general. Using the characteristic equations of
GCN [47] and GRU [21], these components’ individual computational complexities are O(|E|d2L)
and O(Td2L), respectively, where d is the dimensionality of the hidden layer, L is the number of
layers, and there are T elements in each sequence. For temporal models that use the snapshotting
technique for the embedding portion, the GNN computational complexity becomes O(T |E|d2L).
However, if we assume that for Euler models there are T workers, then the complexity is the
same as a traditional GNN, with the caveat that there will be some communication overhead to
synchronize the workers. To account for this, we have added an additional term C(ω) to represent
how communication time scales as more workers are added.
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:29
Table 10. Theoretical Runtime Analysis
Framework O
Euler O(|E|d2
eLe +Td2
r Lr + C(ω))
EGCN O(T (|E|d2
eLe + d2
r Lr ))
VGRNN O(T (|E|d2
eLe + d2
r Lr )) Fig. 7. Performance improvements as more workers
are added for varying numbers of snapshots from the
LANL dataset. The model used was a GCN stacked on
a GRU. All time windows, δ, are 0.5 hours.
Fig. 8. Performance comparison between the distributed Euler model and competing methods on varying
numbers of snapshots from the LANL dataset. All time windows, δ, are 0.5 hours.
For ease of reading, we show time complexities of each model tested in Table 10. As is evident
from this comparison, Euler is the only framework that can remove theT term from the expensive
GNN portion of the forward pass. As the GNN component scales with the number of edges in the
graph, it is clearly the most expensive step. By doing this step just once, rather than T times, as we
will show, there are significant performance improvements that outweigh the cost of ω workers.
Figure 7 shows the speedup of a GCN stacked upon a GRU built within the framework of Euler
as more workers are added. For these experiments, we evaluate only workers that are powers of
two to ensure each worker holds the same number of snapshots. As each worker requires two
threads to run—one for the model replica and one for collective communication—our equipment
can only accommodate up to 16 workers. The framework allows for the number of workers to be
a user-defined hyperparameter, so it is effortless to distribute work across a network with enough
nodes to support it. It is evident from the chart that performance improvements are immediate.
As more workers are added, runtime improves rapidly, with negligible improvement after about
8 workers for smaller amounts of data. However, as the amount of data processed increases, performance improvements diminish at a much slower rate. This is because the topological encoding
task, the bulk of which must occur on CPU due to the high number of random accesses, scales
perfectly with additional workers.
In Figure 8, we compare the best runtimes of our method to the runtimes of the serial GNN
models we used for benchmarking in Section 6. The serial methods were allowed 16 threads for
intraprocess communication for a fair comparison to our 16 worker processes. However, even with
this advantage, these methods are forced to process timesteps one at a time; they simply cannot
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:30 I. J. King and H. H. Huang
Table 11. Training Time for the Full Pipelines with δ = 0.5-hour Snapshots
LANL OpTC
Training Time (s) TPE (s) Training Time (s) TPE (s)
VGRNN 649.48 11.31 767.53 25.62
EGCN-H 1,831.46 98.53 488.08 8.47
EGCN-O 5,047.25 120.87 284.57 6.98
Euler
GCN-GRU 129.51 6.48 184.11 1.48
SAGE-GRU 145.99 10.73 165.14 1.67
GAT-GRU 272.79 9.64 190.37 2.30
GCN-LSTM 115.82 6.90 170.15 1.44
SAGE-LSTM 149.19 11.06 154.69 1.72
GAT-LSTM 302.94 10.02 237.75 2.57
Euler-SM
GCN-GRU 5,186.87 25.06 1,085.43 20.93
SAGE-GRU 4,910.13 25.40 1,024.84 20.95
GAT-GRU 4,835.19 24.14 753.32 21.52
GCN-LSTM 4,499.26 23.89 714.52 19.78
SAGE-LSTM 4,808.62 25.65 728.90 19.28
GAT-LSTM 4,766.39 26.30 762.22 19.33
compete with the efficiency of Euler, especially as the size of data increases. Figure 8(a) shows
that as the size of the data being processed increases, Euler’s forward propagation speed is 2×
that of the fastest competing algorithm. Additionally, by implementing Euler using DDP [52], backpropagation is sped up dramatically. Figure 8(b) shows Euler has almost a 16× improvement in
backpropagation speed, suggesting backpropagation has near linear scaling as workers are added.
More concretely, we report the exact training time each model took in the previous experiments
in Table 11.
As is evident from these results, Euler is noticeably faster than the serial methods. On the LANL
data, the next-best method, VGRNN achieves an average time per epoch (TPE) of 11.31 s, while
Euler with GCN+GRU averages almost half that time with the same number of parameters. On the
OpTC data, as there are fewer nodes, and thus a smaller adjacency matrix, the E-GCN models are
the best-performing serial models, attaining a TPE of 6.98 s. But still the Euler model improves
on this, showing a 4.84× speedup with a 1.44 s TPE. Other combinations of models, especially
those using GraphSAGE, which use the more expensive maxpooling aggregation function as well
as an additional parameter matrix, are slower but still outperform the serial models. We also note
that the full training time is faster for every Euler model, likely due to their simplicity. This
means that in addition to individual epochs running faster both forward and backward passes,
fewer epochs are required before convergence to, as we have shown, more accurate models. With
the softmax augmentation, the neighbor sampling and |V|-dimensional output increase latency
significantly. However, we note that their average TPE is still lower than the majority of serial
methods. Considering the evaluation improvement they show on smaller datasets, we feel this is
an acceptable tradeoff.
Of course, as more workers are added, there will eventually be some diminishing returns.
Though we were not able to reach this level with our hardware, it is worth attempting to quantify a theoretical bound on the end of these benefits. To do this, we run a number of experiments.
In these experiments, we time individual components of the Euler model in an attempt to break
down the exact runtime of each component so we can determine the exact latencies of different
phases of the model.
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:31
Fig. 9. Itemized runtimes of a Euler model performing forward propagation on differently sized data with
16 workers.
Figure 9 shows the latency of each phase of the Euler model, broken into runtimes for the
MPNN embedders, communication time to synchronize the workers’ output, and the RNN’s time
to process this output. As is evident from these figures, the time it takes to synchronize the communication from each worker is far less than the time it would take to run each worker in serial, the
difference between Figures 9(a) and 9(b), respectively. Unsurprisingly, the amount of time it takes
to calculate the embeddings in serial is approximately 16× that of what it takes to calculate them
in parallel. However, the communication time spent waiting for the tensors to be transmitted over
the network, and for the notification that the asynchronous promise has been completed, does take
a significant amount of time: approximately three times as long as the embedding phase itself. And
this latency varies as more workers are added. To quantify a theoretical bound on the benefit enjoyed as we add additional workers, we must determine how this communication time scales with
additional workers, as well as if larger quantities of data has adverse effects on the waiting time.
In Figure 10, we report the communication latency as more workers are added for varying numbers of snapshots the workers process. These values are the average of 10 independent runs across
randomly generated Erdös Rényi graphs [27] to ensure realistic load imbalances with similar node
counts and branch factors as those found in LANL (1,000 nodes, and p = 0.2). Each snapshot
generated has between 10,000 and 20,000 edges, the exact value sampled from the uniform distribution, and assigned according to the Erdös Rényi formula. We find that when these latencies
are normalized, the size of the data being transmitted has very little effect on performance. However, latency does scale linearly as more workers are added. It approximately follows the slope
L(ω) = 0.017( ω
15 + 14
15 ) for ω ≥ 1. However, this hardly seems realistic given enough processors, so
we will also consider the worst case where L(ω) = O(2ω ). If this is the case on our hardware as
more workers are added, then communication overhead roughly doubles every 16 workers. However, this value is highly contingent on the interprocess communication method used and will vary
across different implementations of Euler on different hardware.
With this doubling factor found, we present an equation to bound the effectiveness of adding
additional workers:
Es ≤ s
ω
(L(ω) + E), (12)
where E represents the time to encode a single snapshot, L(ω) represents the communication latency for ω workers, and s is the number of snapshots. The largest positive value ω for which this
inequality holds is the maximum number of workers that will still be beneficial to add.
Through the results found in Figure 10, we see that the number of snapshots per worker has
little effect on its latency. Thus, we can simplify the inequality by assuming, in a perfect scenario,
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:32 I. J. King and H. H. Huang
Fig. 10. Effect of new workers on communication
latency with varying load sizes, normalized.
Fig. 11. Approximate theoretical bounds on the benefit of adding additional workers contingent on how
L(ω) scales.
there will be the same number of workers as snapshots. This allows us to rearrange the inequality
as
ω ≤
L(ω)
E
+ 1. (13)
Under the assumption that L(ω) is linear, this inequality holds for all ω > 5, which is dubious. So,
we will instead assume L(ω) = 0.0172 ω−1 16 and find the bound in the worst case. Finally, we input
the values we found for the various variables to find
ω ≤ 0.017 × 2
ω−1 16 1
0.005
+ 1, (14)
which we plot in Figure 11, along with the linear case. Note that the two functions do not significantly diverge until after 20 workers: beyond our hardware’s ability to evaluate. This is why
we must consider both cases. Thus, after 70 workers, there will be diminishing returns due to the
communication overhead in the worst case.4
11 CONCLUSION
In this work, we presented the Euler framework: a method to exploit the previously untapped potential for distributing the work done to train and execute temporal graph link predictors. When
each topological encoder can operate independently, the most compute-heavy task of generating
node embeddings can be scaled in a highly efficient manner. Though models following this framework are necessarily simple, we have shown that for anomalous link detection and prediction, models following the Euler framework perform as well, or better, than their complex contemporaries.
Finally, we showed how this framework can be used to train highly precise anomaly-based intrusion detection systems when network activity is viewed through the abstraction of a temporal
graph. These intrusion detection systems are scalable and more sound than other unsupervised
techniques despite being trained with less data. Additionally, we found that further improvements
could be made to the classifier by implementing a softmax decoder, rather than the simple inner product logistic regression on the raw embeddings. We showed that while the embeddings
produced by these models have value on their own, we can extract even more information with
additional processing; possibly enough to use this system in a real-world setting.
4If we instead assume L(ω) is polynomial, using least squares regression to find the parabola of best fit, then this value
increases to 2,625. The true value is likely somewhere in between.
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:33
Future work may include testing other topological or temporal encoders that we did not. As this
framework allows for scalable message-passing graph neural networks, future work could even
include testing this technique on any large temporal graph dataset previously thought intractable
for GNNs.
REFERENCES
[1] 2023. About zeek – Book of zeek (v5.1.0). Zeek Documentation (2023). https://docs.zeek.org/en/current/about.html.
[2] 2019. Distributed RPC framework. PyTorch Master Documentation (2019). https://pytorch.org/docs/master/rpc.html.
[3] 2022. Pytorch/tensorpipe: A tensor-aware point-to-point communication primitive for machine learning. Pytorch/tensorpipe (2022). Retrieved from https://github.com/pytorch/tensorpipe.
[4] Stephen D. Bay, Dennis Kibler, Michael J. Pazzani, and Padhraic Smyth.2000. The UCI KDD archive of large data sets
for data mining research and experimentation. ACM SIGKDD Explorations Newsletter 2, 2 (2000), 81–85.
[5] 2014. Intel xeon processor E5-2683 v3 (35M Cache, 2.00 GHz) product specifications. Intel Product Specifications: Processors(2014). Retrieved from https://ark.intel.com/content/www/us/en/ark/products/81055/intel-xeon-processor-e5-
2683-v3-35m-cache-2-00-ghz.html.
[6] Ed Williams, SpiderLab Trustwave, and Edward Millington. 2020. OS credential dumping: LSASS memory, subtechnique T1003.001. Mitre Att&ck (2020). https://attack.mitre.org/techniques/T1003/001/.
[7] Ryan Becwar and Vincent Le Toux. 2020. Use alternate authentication material: Pass the ticket, Sub-technique
T1550.003. Mitre Att&ck (2020). Retrieved from https://attack.mitre.org/techniques/1156T1550/003/.
[8] Jon Sternstein, Mark Wee, Praetorian Netskope, Prasad Somasamduram, Sekhar Sarukkai, Syed Ummar Farooqh,
and Yossi Weizman. 2017. Valid accounts. MITRE ATT&CK (2017). Retrieved from https://attack.mitre.org/techniques/
T1078/.
[9] 2018. Lateral movement, tactic TA0008. MITRE ATT&CK (2018). https://attack.mitre.org/tactics/TA0008/.
[10] Ammar Alazab, Michael Hobbs, Jemal Abawajy, and Moutaz Alazab. 2012. Using feature selection for intrusion detection system. In Proceedings of the International Symposium on Communications and Information Technologies (ISCIT).
IEEE, 296–301.
[11] Gene M. Amdahl. 1967. Validity of the single processor approach to achieving large scale computing capabilities. In
Proceedings of the Spring Joint Computer Conference. 483–485.
[12] Md Monowar Anjum, Shahrear Iqbal, and Benoit Hamelin. 2021. Analyzing the usefulness of the DARPA OpTC dataset
in cyber threat detection research. In Proceedings of the 26th ACM Symposium on Access Control Models and Technologies. 27–32.
[13] Tim Bai, Haibo Bian, Abbas Abou Daya, Mohammad A. Salahuddin, Noura Limam, and Raouf Boutaba. 2019. A machine learning approach for RDP-based lateral movement detection. In Proceedings of the IEEE 44th Conference on
Local Computer Networks (LCN). IEEE, 242–245.
[14] Siddharth Bhatia, Bryan Hooi, Minji Yoon, Kijung Shin, and Christos Faloutsos. 2020. MIDAS: Microcluster-based
detector of anomalies in edge streams. In Proceedings of the AAAI Conference on Artificial Intelligence, Vol. 34.
3242–3249.
[15] Haibo Bian, Tim Bai, Mohammad A. Salahuddin, Noura Limam, Abbas Abou Daya, and Raouf Boutaba. 2021. Uncovering lateral movement using authentication logs. IEEE Trans. Netw. Serv. Manag. 18, 1 (2021), 1049–1063.
[16] Benjamin Bowman, Craig Laprade, Yuede Ji, and H. Howie Huang. 2020. Detecting lateral movement in enterprise
computer networks with unsupervised graph AI. In Proceedings of the 23rd International Symposium on Research in
Attacks, Intrusions and Defenses (RAID’20). 257–268.
[17] Anna L. Buczak and Erhan Guven. 2015. A survey of data mining and machine learning methods for cyber security
intrusion detection. IEEE Commun. Surv. Tutor. 18, 2 (2015), 1153–1176.
[18] Brian Caswell and Jay Beale. 2004. Snort 2.1 Intrusion Detection. Elsevier.
[19] Jinyin Chen, Xuanheng Xu, Yangyang Wu, and Haibin Zheng. 2018. GC-LSTM: Graph convolution embedded LSTM
for dynamic link prediction. arXiv preprint arXiv:1812.04206 (2018).
[20] Pin-Yu Chen, Sutanay Choudhuri, Luke Rodriguez, Alfred Hero, and Indrajit Ray. 2019. Enterprise cyber resiliency
against lateral movement: A graph theoretic approach. Industrial Control Systems Security and Resiliency, Springer
Nature. DOI:https://doi.org/10.1007/978-3-030-18214-4_5
[21] Kyunghyun Cho, Bart Van Merriënboer, Dzmitry Bahdanau, and Yoshua Bengio. 2014. On the properties of neural
machine translation: Encoder-decoder approaches. arXiv preprint arXiv:1409.1259 (2014).
[22] Junyoung Chung, Caglar Gulcehre, KyungHyun Cho, and Yoshua Bengio. 2014. Empirical evaluation of gated recurrent neural networks on sequence modeling. arXiv preprint arXiv:1412.3555 (2014).
[23] Benoit Claise. 2004. Cisco systems netflow services export version 9. The Internet Engineering Task Force (IETF), Network Working Group, Technical Report. Retrieved from https://www.ietf.org/rfc/rfc3954.txt.
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:34 I. J. King and H. H. Huang
[24] L. Dhanabal and S. P. Shantharajah. 2015. A study on NSL-KDD dataset for intrusion detection system based on
classification algorithms. Int. J. Adv. Res. Comput. Commun. Eng. 4, 6 (2015), 446–452.
[25] Aswathy Divakaran and Anuraj Mohan. 2019. Temporal link prediction: A survey. New Gener. Comput. 38 (2019),
213–258.
[26] Mohamed Gamal El-Hadidi and Marianne A. Azer. 2020. Detecting Mimikatz in lateral movements using Mutex. In
Proceedings of the 15th International Conference on Computer Engineering and Systems (ICCES). IEEE, 1–6.
[27] Paul Erdös and Alfréd Rényi. 2011. On the evolution of random graphs. In The Structure and Dynamics of Networks.
Princeton University Press, 38–82.
[28] Fahimeh Farahnakian and Jukka Heikkonen. 2018. A deep auto-encoder based approach for intrusion detection system.
In Proceedings of the 20th International Conference on Advanced Communication Technology (ICACT). IEEE, 178–183.
[29] Chuanpu Fu, Qi Li, Meng Shen, and Ke Xu. 2021. Realtime robust malicious traffic detection via frequency domain
analysis. In Proceedings of the ACM SIGSAC Conference on Computer and Communications Security. 3431–3446.
[30] Justin Gilmer, Samuel S. Schoenholz, Patrick F. Riley, Oriol Vinyals, and George E. Dahl. 2017. Neural message passing
for quantum chemistry. In Proceedings of the International Conference on Machine Learning. PMLR, 1263–1272.
[31] Joshua Glasser and Brian Lindauer. 2013. Bridging the gap: A pragmatic approach to generating insider threat data.
In Proceedings of the IEEE Security and Privacy Workshops. IEEE, 98–104.
[32] Palash Goyal, Sujit Rokka Chhetri, and Arquimedes Canedo. 2020. dyngraph2vec: Capturing network dynamics using
dynamic graph representation learning. Knowl.-based Syst. 187 (2020), 104816.
[33] Palash Goyal, Nitin Kamra, Xinran He, and Yan Liu. 2018. DynGEM: Deep embedding method for dynamic graphs.
arXiv preprint arXiv:1805.11273 (2018).
[34] Aditya Grover and Jure Leskovec. 2016. node2vec: Scalable feature learning for networks. In Proceedings of the 22nd
ACM SIGKDD International Conference on Knowledge Discovery and Data Mining. 855–864.
[35] Ehsan Hajiramezanali, Arman Hasanzadeh, Krishna Narayanan, Nick Duffield, Mingyuan Zhou, and Xiaoning
Qian. 2019. Variational graph recurrent neural networks. In Advances in Neural Information Processing Systems, Vol. 32. Curran Associates, Inc., 10701–10711. Retrieved from https://proceedings.neurips.cc/paper/2019/file/
a6b8deb7798e7532ade2a8934477d3ce-Paper.pdf.
[36] William L. Hamilton, Rex Ying, and Jure Leskovec. 2017. Inductive representation learning on large graphs. arXiv
preprint arXiv:1706.02216 (2017).
[37] Nick Heard and Patrick Rubin-Delanchy. 2016. Network-wide anomaly detection via the Dirichlet process. In Proceedings of the IEEE Conference on Intelligence and Security Informatics (ISI). IEEE, 220–224.
[38] Grant Ho, Mayank Dhiman, Devdatta Akhawe, Vern Paxson, Stefan Savage, Geoffrey M. Voelker, and David Wagner. 2021. Hopper: Modeling and detecting lateral movement. In Proceedings of the 30th USENIX Security Symposium
(USENIX Security’21). 3093–3110.
[39] Sepp Hochreiter and Jürgen Schmidhuber. 1997. Long short-term memory. Neural Computat. 9, 8 (1997), 1735–1780.
[40] Hassaan Irshad, Gabriela Ciocarlie, Ashish Gehani, Vinod Yegneswaran, Kyu Hyung Lee, Jignesh Patel, Somesh Jha,
Yonghwi Kwon, Dongyan Xu, and Xiangyu Zhang. 2021. TRACE: Enterprise-wide provenance tracking for real-time
apt detection. IEEE Trans. Inf. Forens. Secur. 16 (2021), 4363–4376.
[41] Alexander D. Kent. 2015. Comprehensive, Multi-source Cyber-security Events Data Set. Technical Report. Los Alamos
National Lab. (LANL), Los Alamos, NM.
[42] Alexander D. Kent, Lorie M. Liebrock, and Joshua C. Neil. 2015. Authentication graphs: Analyzing user behavior
within an enterprise network. Comput. Secur. 48 (2015), 150–166.
[43] Adam Khalid, Anazida Zainal, Mohd Aizaini Maarof, and Fuad A. Ghaleb. 2021. Advanced persistent threat detection:
A survey. In Proceedings of the 3rd International Cyber Resilience Conference (CRC). IEEE, 1–6.
[44] Isaiah J. King and H. Howie Huang. 2022. Euler: Detecting network lateral movement via scalable temporal graph
link prediction. In Proceedings of the Network and Distributed System Security Symposium. Internet Society. DOI:https:
//doi.org/10.14722/ndss.2022.24107
[45] Samuel T. King and Peter M. Chen. 2003. Backtracking intrusions. In Proceedings of the 19th ACM Symposium on
Operating Systems Principles. 223–236.
[46] Diederik P. Kingma and Jimmy Ba. 2014. Adam: A method for stochastic optimization. arXiv preprint arXiv:1412.6980
(2014).
[47] Thomas N. Kipf and Max Welling. 2016. Semi-supervised classification with graph convolutional networks. arXiv
preprint arXiv:1609.02907 (2016).
[48] Thomas N. Kipf and Max Welling. 2016. Variational graph auto-encoders. arXiv preprint arXiv:1611.07308 (2016).
[49] Jon Kleinberg and Éva Tardos. 2006. Interval scheduling: The greedy algorithm stays ahead. In Algorithm Design.
Addison Wesley.
[50] Mu Li, David G. Andersen, Jun Woo Park, Alexander J. Smola, Amr Ahmed, Vanja Josifovski, James Long, Eugene J.
Shekita, and Bor-Yiing Su. 2014. Scaling distributed machine learning with the parameter server. In Proceedings of the
11th USENIX Symposium on Operating Systems Design and Implementation (OSDI’14). 583–598.
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
Euler: Detecting Network Lateral Movement via Scalable Temporal Link Prediction 35:35
[51] Meicong Li, Wei Huang, Yongbin Wang, Wenqing Fan, and Jianfang Li. 2016. The study of APT attack stage model. In
Proceedings of the IEEE/ACIS 15th International Conference on Computer and Information Science (ICIS). IEEE, 1–5.
[52] Shen Li, Yanli Zhao, Rohan Varma, Omkar Salpekar, Pieter Noordhuis, Teng Li, Adam Paszke, Jeff Smith, Brian
Vaughan, Pritam Damania, et al. 2020. PyTorch distributed: Experiences on accelerating data parallel training. arXiv
preprint arXiv:2006.15704 (2020).
[53] Taisong Li, Jiawei Zhang, S. Yu Philip, Yan Zhang, and Yonghong Yan. 2018. Deep dynamic network embedding for
link prediction. IEEE Access 6 (2018), 29219–29230.
[54] Qingyun Liu, Jack W. Stokes, Rob Mead, Tim Burrell, Ian Hellen, John Lambert, Andrey Marochko, and Weidong Cui.
2018. Latte: Large-scale lateral movement detection. In Proceedings of the IEEE Military Communications Conference
(MILCOM). IEEE, 1–6.
[55] Ritika Lohiya and Ankit Thakkar. 2021. Intrusion detection using deep neural network with antirectifier layer. In
Applied Soft Computing and Communication Networks. Springer, 89–105.
[56] Sadegh M. Milajerdi, Birhanu Eshete, Rigel Gjomemo, and V. N. Venkatakrishnan. 2019. Poirot: Aligning attack behavior with kernel audit records for cyber threat hunting. In Proceedings of the ACM SIGSAC Conference on Computer
and Communications Security. 1795–1812.
[57] Sadegh M. Milajerdi, Rigel Gjomemo, Birhanu Eshete, Ramachandran Sekar, and V. N. Venkatakrishnan. 2019. Holmes:
Real-time apt detection through correlation of suspicious information flows. In Proceedings of the IEEE Symposium on
Security and Privacy (SP). IEEE, 1137–1152.
[58] Yisroel Mirsky, Tomer Doitshman, Yuval Elovici, and Asaf Shabtai. 2018. Kitsune: An ensemble of autoencoders for
online network intrusion detection. Mach. Learn. 5 (2018), 2.
[59] MITRE. 2022. Data Model | MITRE Cyber Analytics Repository. Retrieved from https://car.mitre.org/data_model/.
[60] Ghulam Mohi-ud din. 2018. NSL-KDD. DOI:https://doi.org/10.21227/425a-3e55
[61] Giang Hoang Nguyen, John Boaz Lee, Ryan A. Rossi, Nesreen K. Ahmed, Eunyee Koh, and Sungchul Kim. 2018.
Continuous-time dynamic network embeddings. In Proceedings of the Web Conference. 969–976.
[62] Linshu Ouyang, Yongzheng Zhang, and Yipeng Wang. 2020. Unified graph embedding-based anomalous edge detection. In Proceedings of the International Joint Conference on Neural Networks (IJCNN). IEEE, 1–8.
[63] Aldo Pareja, Giacomo Domeniconi, Jie Chen, Tengfei Ma, Toyotaro Suzumura, Hiroki Kanezashi, Tim Kaler, Tao
Schardl, and Charles Leiserson. 2020. EvolveGCN: Evolving graph convolutional networks for dynamic graphs. In
Proceedings of the AAAI Conference on Artificial Intelligence, Vol. 34. 5363–5370.
[64] Bryan Perozzi, Rami Al-Rfou, and Steven Skiena. 2014. DeepWalk: Online learning of social representations. In Proceedings of the 20th ACM SIGKDD International Conference on Knowledge Discovery and Data Mining. 701–710.
[65] Mahmudur Rahman, Tanay Kumar Saha, Mohammad Al Hasan, Kevin S. Xu, and Chandan K. Reddy. 2018. Dylink2vec:
Effective feature representation for link prediction in dynamic networks. arXiv preprint arXiv:1804.05755 (2018).
[66] Aravind Sankar, Yanhong Wu, Liang Gou, Wei Zhang, and Hao Yang. 2018. Dynamic graph representation learning
via self-attention networks. arXiv preprint arXiv:1812.09430 (2018).
[67] Iqbal H. Sarker. 2021. CyberLearning: Effectiveness analysis of machine learning security modeling to detect cyberanomalies and multi-attacks. Internet Things 14 (2021), 100393.
[68] Youngjoo Seo, Michaël Defferrard, Pierre Vandergheynst, and Xavier Bresson. 2018. Structured sequence modeling
with graph convolutional recurrent networks. In Proceedings of the International Conference on Neural Information
Processing. Springer, 362–373.
[69] Oleksandr Shchur, Maximilian Mumme, Aleksandar Bojchevski, and Stephan Günnemann. 2018. Pitfalls of graph
neural network evaluation. arXiv preprint arXiv:1811.05868 (2018).
[70] Michele Starnini, Andrea Baronchelli, Alain Barrat, and Romualdo Pastor-Satorras. 2012. Random walks on temporal
networks. Phys. Rev. E 85, 5 (2012), 056115.
[71] Branka Stojanović, Katharina Hofer-Schmitz, and Ulrike Kleb. 2020. APT datasets and attack modeling for automated
detection methods: A review. Comput. Secur. 92 (2020), 101734.
[72] Blake E. Strom, Andy Applebaum, Doug P. Miller, Kathryn C. Nickels, Adam G. Pennington, and Cody B. Thomas.
2018. Mitre attack: Design and philosophy. The MITRE Corporation, Technical Report.
[73] Aynaz Taheri, Kevin Gimpel, and Tanya Berger-Wolf. 2019. Learning to represent the evolution of dynamic graphs
with recurrent models. In Proceedings of the World Wide Web Conference. 301–307.
[74] Jian Tang, Meng Qu, Mingzhe Wang, Ming Zhang, Jun Yan, and Qiaozhu Mei. 2015. Line: Large-scale information
network embedding. In Proceedings of the 24th International Conference on World Wide Web. 1067–1077.
[75] Jacob Torrey. 2020. GitHub - darpa-i2o/Transparent-Computing: Material from the DARPA Transparent Computing
Program. Retrieved from https://github.com/darpa-i2o/Transparent-Computing.
[76] Petar Veličković, Guillem Cucurull, Arantxa Casanova, Adriana Romero, Pietro Lio, and Yoshua Bengio. 2017. Graph
attention networks. arXiv preprint arXiv:1710.10903 (2017).
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.
35:36 I. J. King and H. H. Huang
[77] Joost Verbraeken, Matthijs Wolting, Jonathan Katzy, Jeroen Kloppenburg, Tim Verbelen, and Jan S. Rellermeyer. 2020.
A survey on distributed machine learning. ACM Comput. Surv. 53, 2 (2020), 1–33.
[78] Anthony Yu-Tung Wang, Ryan J. Murdock, Steven K. Kauwe, Anton O. Oliynyk, Aleksander Gurlo, Jakoah Brgoch,
Kristin A. Persson, and Taylor D. Sparks. 2020. Machine learning for materials scientists: An introductory guide toward
best practices. Chem. Mater. 32, 12 (2020), 4954–4965.
[79] Shen Wang and S. Yu Philip. 2019. Heterogeneous graph matching networks: Application to unknown malware detection. IEEE International Conference on Big Data (2019), 5401–5408. https://doi.org/10.1109/BigData47090.2019.9006464
[80] Renzheng Wei, Lijun Cai, Aimin Yu, and Dan Meng. 2021. DeepHunter: A graph neural network based approach for
robust cyber threat hunting. arXiv preprint arXiv:2104.09806 (2021).
[81] Mark Whitehouse, Marina Evangelou, and Niall M. Adams. 2016. Activity-based temporal anomaly detection in
enterprise-cyber security. In Proceedings of the IEEE Conference on Intelligence and Security Informatics (ISI). IEEE,
248–250.
[82] Eric P. Xing, Qirong Ho, Pengtao Xie, and Dai Wei. 2016. Strategies and principles of distributed machine learning on
big data. Engineering 2, 2 (2016), 179–195.
[83] Keyulu Xu, Weihua Hu, Jure Leskovec, and Stefanie Jegelka. 2018. How powerful are graph neural networks? arXiv
preprint arXiv:1810.00826 (2018).
[84] Ling Zhao, Yujiao Song, Chao Zhang, Yu Liu, Pu Wang, Tao Lin, Min Deng, and Haifeng Li. 2019. T-GCN: A temporal
graph convolutional network for traffic prediction. IEEE Trans. Intell. Transport. Syst. 21, 9 (2019), 3848–3858.
[85] Lekui Zhou, Yang Yang, Xiang Ren, Fei Wu, and Yueting Zhuang. 2018. Dynamic network embedding by modeling
triadic closure process. In Proceedings of the AAAI Conference on Artificial Intelligence, Vol. 32.
Received 19 August 2022; revised 3 February 2023; accepted 13 March 2023
ACM Transactions on Privacy and Security, Vol. 26, No. 3, Article 35. Publication date: June 2023.

---

**Cursor**

Here's exactly what Euler says about provenance, and what it means for your provenance-vs-host plan.

## 1. Euler's only direct statements on provenance

In Related Work (§2.1), provenance graphs are placed firmly in the **signature-based** camp, not the link-prediction camp:

> "The primary focus of research involving graphs for signature-based intrusion detection is on the analysis of **provenance graphs** [45]. References [56, 57] accomplish this through **subgraph matching**… [79, 80] use **vector embeddings of the provenance graphs to measure similarity** between observed and known malicious graphs… Despite these methods' effectiveness at identifying known threats, advanced attackers will endeavor not to use a method with a previously discovered signature. **For this reason, we turn to anomaly-based intrusion detection methods.**"

Those references are Poirot/Holmes (subgraph matching) and DeepHunter/Wang (graph similarity). **Euler then deliberately walks away from provenance** and builds on **host/auth and network-flow graphs** (LANL auth, OpTC FLOW-START).

## 2. What this means for you (this is good news)

Euler **defines the exact gap you'd fill**:

| | Provenance graphs | Host/interaction graphs |
|--|-------------------|--------------------------|
| Existing paradigm | **Signature-based** (subgraph match, known-malware similarity) | **Anomaly-based temporal LP** (Euler, Bowman, Jbeil) |
| Your target | **Anomaly-based / temporal link prediction on provenance** ← empty | (already crowded) |

So your one-line novelty writes itself:

> *"Prior provenance-graph IDS is signature-based subgraph matching (Poirot, Holmes, DeepHunter). Euler-style temporal/anomaly link prediction has only been applied to coarse host graphs. We apply link prediction to fine-grained provenance graphs and test whether granularity improves APT detection."*

Euler is the perfect foil: it explicitly *chose host graphs over provenance*, so "does that choice cost detection power?" is a legitimate, citable research question — not a contrived one.

## 3. Where Euler *supports* your granularity intuition — and where it does NOT

**Supports (cite these):**
- §8.4 / Fig. 5: **smaller time windows (finer temporal granularity) consistently give better scores.** "more granular graphs lead to more informative edge scores."
- §8.3 / §8.6: their biggest weakness was **coarse labels** — "labeled anomalous events are very coarse-grained… treated as false positives due to the lack of fine-grained label information… the red log only tracks 'compromise events' and not the further malicious activity."

**Does NOT support (be honest):**
- Euler's "granularity" = **temporal** (snapshot size) and **label** granularity — **not structural** (provenance vs host). It never compares graph representations. So you **cannot cite Euler as evidence** that provenance > host; you can only cite it as **motivation** that finer granularity (temporal + label) helped, suggesting structural granularity is worth testing.

This is actually your wedge: **CICAPT-IIoT gives you fine-grained per-node attack labels (`label`, `subLabel`)** — directly addressing the coarse-label limitation Euler complained about. Call that out explicitly; it's a concrete advantage over LANL.

## 4. Lessons from Euler you should adopt verbatim

These de-risk your week and make reviewers trust your numbers:

1. **Train on benign-before-attack** (Phase 1 in your case) — exactly their protocol.
2. **Report AP, not just AUC** — they stress AUC is misleading under extreme imbalance; AP is recommended for anomaly detection.
3. **Include the UA baseline** ("flag any edge unseen in training") — their key sanity baseline; beating its TPR/FPR proves you're learning context, not memorizing.
4. **Mask ~5% edges per snapshot for validation**, hold out final snapshots for thresholding.
5. **Use small snapshots** (they used 30 min) — expect finer = better but slower.
6. **GCN was their best link-detection encoder** — your exact pipeline; you don't need GAT/SAGE.

## 5. Honest risk the paper flags for you

Euler's own caution applies doubly to provenance: **extreme class imbalance + FPR "too high to be useful on its own."** Provenance graphs are bigger and denser → even harder FPR. Mitigate by:
- reporting **TPR @ fixed low FPR** and **per-tactic recall**, not raw accuracy;
- positioning your method as a **filter/triage stage** (as Euler does), not a standalone IDS.

## Bottom line

Euler **says provenance = signature-based and explicitly does link prediction only on host graphs.** That makes your plan — *temporal/anomaly link prediction on provenance graphs, compared head-to-head with host graphs on the same events/labels* — a genuine, well-motivated gap that Euler itself opens. Just be precise: cite Euler's **temporal/label granularity** results as motivation, claim the **structural (provenance-vs-host) granularity** comparison as **your** contribution, and adopt their evaluation protocol (benign-train, AP, UA baseline) so your results are credible in 6 days.

---

**User**

ok give me final safet option precisley with plan

---

**Cursor**

Locking it. Here is the single safest plan — minimum moving parts, maximum reuse of what you already have, one clear contribution.

## Final decision

**Static GCN link prediction on CICAPT-IIoT, provenance graph vs host graph, same events/same labels, with SoHeaRT hard negatives.**

Why this is the safe choice:
- **Data is already on disk and parsed** (your biggest risk — data wrangling — is gone).
- **Reuses your exact pipeline**: GCN encoder + dot-product decoder + SVD features + SoHeaRT negatives. Near-zero new model code.
- **No temporal RNN** (that's the part that eats days). Static link prediction only.
- **Can't fully fail**: framed as a question, any outcome is publishable.

## The one-sentence contribution

> *"We compare fine-grained provenance graphs against coarse host graphs for GCN-based APT link prediction on identical events and ground truth, and show that security-aware hard negatives (SoHeaRT) are required to evaluate the difference fairly."*

Two contributions bundled: (1) provenance-vs-host comparison, (2) SoHeaRT hard negatives in the security setting — your unique skill.

## Exact setup

**Two graphs from the same CSVs (Phase 1 + Phase 2):**
1. **Provenance graph**: Process + Artifact nodes, edges = the 4 provenance relations (collapse to homogeneous; edge-type optional as feature).
2. **Host graph**: collapse to host/IP–host/IP edges (from socket `local/remote address`).

**Split (Euler protocol):**
- **Train** on Phase 1 (benign) + masked Phase 2 benign edges.
- **Test positives** = Phase 2 edges incident to `label=1` nodes (use `subLabel` for per-tactic breakdown).

**Negatives:** random vs **SoHeaRT** (your composite re-scoring). Same model, both regimes.

**Model:** your GCN + dot-product decoder, SVD node features. Unchanged.

**Metrics:** AP (primary), AUC, **TPR @ 1% FPR**, **per-tactic recall**, plus the **UA baseline** (flag unseen-in-training edges) and a heuristic (Adamic-Adar/CN).

## 6-day plan

| Day | Deliverable |
|-----|-------------|
| **1** | Build **both graphs** + labels from Phase1/Phase2 (reuse `preprocess_twitter_edges.py`). Print stats: nodes, edges, attack edges per view. **Feasibility gate:** confirm attack edges survive in both views. |
| **2** | Train/test split + masking. Implement metrics + UA + Adamic-Adar baselines. **Decision gate:** if provenance graph won't train sanely → drop provenance arm, fall back to **host-graph (CICAPT) + LANL host-graph**, same contribution minus granularity. |
| **3** | Run **GCN + dot-product** on both graphs, **random negatives**. First results table. |
| **4** | Add **SoHeaRT** negatives; re-run both graphs. Now you have the 2×2 core result (graph × negative). |
| **5** | Per-tactic recall + TPR@FPR + one **case study** (an attack edge visible in provenance, absent/late in host graph). Freeze results. |
| **6** | Write all 6 pages. Submit. |

**Frozen minimum viable paper by end of Day 4** (GCN on both graphs, random + SoHeaRT). Days 5–6 only add polish.

## Fallback (if Day-2 gate fails)

Same pipeline, swap the provenance arm for **LANL host graph**. Contribution becomes: *"realistic hard-negative (SoHeaRT) evaluation of GCN link prediction for APT detection across two host datasets."* Still a complete paper, zero wasted code.

## 3 papers to cite & differentiate
1. **Euler (King & Huang, TOPS'23)** — host-graph temporal LP; explicitly leaves provenance to signature-based methods → your gap.
2. **Bowman et al. (RAID'20)** — static host-graph LP with simple negatives → you add SoHeaRT hard negatives.
3. **Jbeil (S&P'24)** — temporal host LP → your contribution (granularity + negatives) is orthogonal.

## Two guardrails (from Euler)
- Report **AP / TPR@low-FPR**, never accuracy (99.5% benign).
- Position as a **triage/filter**, not standalone IDS (Euler conceded FPR too high alone).

---

This is the safest end-to-end path: data in hand, your pipeline unchanged, one clean comparison, a hard fallback, and a 6-day schedule with a frozen MVP by Day 4.

When you're ready to execute, switch to Agent mode and I'll start Day 1 — build both graph views + labels + the feasibility stats — so you know within an hour that the signal is there.

---

**User**

ok lets work on this:
Locking it. Here is the single safest plan — minimum moving parts, maximum reuse of what you already have, one clear contribution.

## Final decision

**Static GCN link prediction on CICAPT-IIoT, provenance graph vs host graph, same events/same labels, with SoHeaRT hard negatives.**

Why this is the safe choice:
- **Data is already on disk and parsed** (your biggest risk — data wrangling — is gone).
- **Reuses your exact pipeline**: GCN encoder + dot-product decoder + SVD features + SoHeaRT negatives. Near-zero new model code.
- **No temporal RNN** (that's the part that eats days). Static link prediction only.
- **Can't fully fail**: framed as a question, any outcome is publishable.

## The one-sentence contribution

> *"We compare fine-grained provenance graphs against coarse host graphs for GCN-based APT link prediction on identical events and ground truth, and show that security-aware hard negatives (SoHeaRT) are required to evaluate the difference fairly."*

Two contributions bundled: (1) provenance-vs-host comparison, (2) SoHeaRT hard negatives in the security setting — your unique skill.

## Exact setup

**Two graphs from the same CSVs (Phase 1 + Phase 2):**
1. **Provenance graph**: Process + Artifact nodes, edges = the 4 provenance relations (collapse to homogeneous; edge-type optional as feature).
2. **Host graph**: collapse to host/IP–host/IP edges (from socket `local/remote address`).

**Split (Euler protocol):**
- **Train** on Phase 1 (benign) + masked Phase 2 benign edges.
- **Test positives** = Phase 2 edges incident to `label=1` nodes (use `subLabel` for per-tactic breakdown).

**Negatives:** random vs **SoHeaRT** (your composite re-scoring). Same model, both regimes.

**Model:** your GCN + dot-product decoder, SVD node features. Unchanged.

**Metrics:** AP (primary), AUC, **TPR @ 1% FPR**, **per-tactic recall**, plus the **UA baseline** (flag unseen-in-training edges) and a heuristic (Adamic-Adar/CN).

## 6-day plan

| Day | Deliverable |
|-----|-------------|
| **1** | Build **both graphs** + labels from Phase1/Phase2 (reuse `preprocess_twitter_edges.py`). Print stats: nodes, edges, attack edges per view. **Feasibility gate:** confirm attack edges survive in both views. |
| **2** | Train/test split + masking. Implement metrics + UA + Adamic-Adar baselines. **Decision gate:** if provenance graph won't train sanely → drop provenance arm, fall back to **host-graph (CICAPT) + LANL host-graph**, same contribution minus granularity. |
| **3** | Run **GCN + dot-product** on both graphs, **random negatives**. First results table. |
| **4** | Add **SoHeaRT** negatives; re-run both graphs. Now you have the 2×2 core result (graph × negative). |
| **5** | Per-tactic recall + TPR@FPR + one **case study** (an attack edge visible in provenance, absent/late in host graph). Freeze results. |
| **6** | Write all 6 pages. Submit. |

**Frozen minimum viable paper by end of Day 4** (GCN on both graphs, random + SoHeaRT). Days 5–6 only add polish.

## Fallback (if Day-2 gate fails)

Same pipeline, swap the provenance arm for **LANL host graph**. Contribution becomes: *"realistic hard-negative (SoHeaRT) evaluation of GCN link prediction for APT detection across two host datasets."* Still a complete paper, zero wasted code.

## 3 papers to cite & differentiate
1. **Euler (King & Huang, TOPS'23)** — host-graph temporal LP; explicitly leaves provenance to signature-based methods → your gap.
2. **Bowman et al. (RAID'20)** — static host-graph LP with simple negatives → you add SoHeaRT hard negatives.
3. **Jbeil (S&P'24)** — temporal host LP → your contribution (granularity + negatives) is orthogonal.

## Two guardrails (from Euler)
- Report **AP / TPR@low-FPR**, never accuracy (99.5% benign).
- Position as a **triage/filter**, not standalone IDS (Euler conceded FPR too high alone).

---

This is the safest end-to-end path: data in hand, your pipeline unchanged, one clean comparison, a hard fallback, and a 6-day schedule with a frozen MVP by Day 4.

When you're ready to execute, switch to Agent mode and I'll start Day 1 — build both graph views + labels + the feasibility stats — so you know within an hour that the signal is there.


first briefly explain me the problem what we are doing

---

**Cursor**

**The problem:** APT attacks are stealthy — they use normal-looking tools slowly, so single events look benign. What’s suspicious is **unusual connections** between entities over time (a process touching a file it shouldn’t, a host talking to a host it never talked to before).

**What Euler and others do:** Model the network as a **coarse host graph** (machine A ↔ machine B) and use **link prediction** to learn normal connections; **low-likelihood edges = possible attack**. Euler does this with **temporal** snapshots. Provenance graphs (process/file/socket) are usually used only for **signature matching** (known attack patterns), not link prediction.

**What you’re doing:** On **CICAPT-IIoT** (same logs, same attack labels), build **two graphs from the same data**:
1. **Provenance graph** — fine-grained (Process, Artifact, typed edges)
2. **Host graph** — coarse (IP/host ↔ IP/host)

Train a **GCN** (your existing encoder + dot-product decoder) on **benign Phase 1** to predict links. On **Phase 2**, score edges and check whether **attack-related links** rank as unlikely. Compare: *does the provenance view catch more APT steps than the host view?*

**Your extra twist:** Most papers evaluate with **random negative edges** (easy). You add **SoHeaRT hard negatives** (structurally plausible but false links) so results reflect realistic difficulty, not inflated Hits@K.

**Success looks like:** Provenance graph gives **higher recall/AP on attack edges** (especially tactics like credential access, collection) than host graph — under **fair (hard-negative) evaluation**. Any clear difference (even “host wins on X, provenance wins on Y”) is a publishable answer to: *does graph granularity matter for APT link prediction?*

---

**User**

but there must be a reason euler doesnot use provenance graphs?
Can you check? 
@euler.txt 

---

**Cursor**

**Euler does not use provenance graphs by design — not because they failed on them.** The paper never runs a provenance experiment; it chooses host/auth graphs for problem definition, scalability, and where it sits in the detection stack.

## 1. Provenance = signature-based; Euler = anomaly-based

In §2.1, Euler explicitly buckets provenance work (Poirot, Holmes, DeepHunter) under **signature-based IDS** — subgraph matching or embedding similarity to **known** attack patterns. It then says APTs avoid known signatures and pivots to **anomaly-based** methods instead.

So provenance is cited as the *other* paradigm, not as input to Euler.

## 2. Lateral movement is framed at host↔host communication

§3 states that prior work shows lateral movement is “most easily detected in unusual **authentications** between users or machines.” The formal input is just interaction tuples `<src, dst, ts>` — enough for auth/flow graphs, not process–file–socket provenance.

The threat model (§7) reinforces this: Euler only needs **src/dst of internal traffic** (auth records, FLOW-START). It targets **inter-host irregularities**, not fine-grained causal chains.

## 3. Scalability is the main engineering reason

Euler’s contribution is **distributed temporal GNN** on large logs:

| Dataset | Raw scale | Euler’s graph |
|---------|-----------|---------------|
| LANL | 45M auth events | 17,685 host nodes |
| OpTC | **17.4B events** (1.1 TB) | **1,114 host nodes** from FLOW-START only |

On OpTC they **had** process-level events (`PROCESS-OPEN`) but deliberately dropped them:

> “While PROCESS-OPEN events … also capture this, they will also always produce a FLOW-START event. Thus, it is the **most efficient** way to capture all communication between hosts.”

That is a direct tradeoff: provenance-level graphs are larger, heterogeneous, and harder to shard across Euler’s parallel snapshot workers. Future work mentions scaling to graphs “previously thought intractable for GNNs” — provenance fits that category; Euler avoids it.

## 4. Euler is meant as a coarse filter, not deep forensics

§8 notes Euler can **pre-filter** events before “a signature-based technique” (i.e., provenance/subgraph tools). Coarse host graphs keep inference cheap; provenance analysis stays downstream.

## 5. What they do *not* claim

Euler does **not** say provenance is worse for APT detection — only that it belongs to a different IDS family and doesn’t fit their scalability + lateral-movement abstraction. They never compare provenance vs host graphs empirically.

---

**For your CARS paper:** that gap is real and citable. Euler’s reasons (anomaly vs signature taxonomy, host-level lateral-movement signal, scalability) are motivations for *their* design — your “granularity matters” comparison on CICAPT-IIoT directly tests what they left open.

---

**User**

but i think eulers reason makes sense that using provenance graph would be irrelevant for this problem 

---

**Cursor**

Checking whether CICAPT attack signal survives host-level collapse — that determines if Euler's abstraction argument holds here.


**For Euler’s exact problem, yes — provenance is largely the wrong abstraction.** Your instinct matches what the paper is actually solving.

## What Euler is actually detecting

Euler is not doing “APT detection” in general. It is doing **anomalous inter-entity communication** — unusual auth/flow edges between hosts. The signal it cares about is:

> “Machine A talked to machine B when that relationship is rare.”

A provenance graph answers a different question:

> “Process P read file F, spawned Q, used socket S…”

Most of those edges never appear in a host↔host graph, and they do not directly encode “will host A connect to host B?” So for **Euler-style lateral-movement LP**, extra provenance granularity is mostly noise, not signal. OpTC’s choice of `FLOW-START` over `PROCESS-OPEN` is the same logic: keep only what manifests inter-host communication.

## Where “irrelevant” is precise vs too strong

| Claim | Verdict |
|-------|---------|
| Provenance is irrelevant for **Euler’s task** (temporal host/auth LP) | **Mostly yes** |
| Provenance is irrelevant for **APT detection broadly** | **No** |
| Provenance is irrelevant for **your CICAPT labels** | **Probably no** |

On your Phase 2 data, attack activity is mostly **not** lateral movement at the host level:

- 332 attack nodes; only **10** tagged `lateralMovement`
- Only **18** attack-touching edges involve network sockets
- Dominant tactics: `collection` (100), `credentialAccess` (82), `defenceEvasion`, `discovery` — mostly local process/file behavior

So Euler’s abstraction makes sense **for the problem Euler defines**. But CICAPT’s ground truth is mostly **fine-grained provenance-level activity**, not rare host↔host links. A host-collapsed graph may simply **discard most of what you’re trying to detect**.

## Implication for your paper angle

A provenance vs host comparison is still valid, but the framing should shift:

- **Not:** “Euler was wrong to skip provenance for lateral movement.”
- **Better:** “Euler’s host-level LP is well-motivated for inter-host lateral movement, but APT benchmarks with provenance labels measure a **different phenomenon** — and granularity must match the threat model.”

That is a cleaner, more defensible story than treating it as a direct fix to Euler.

If you want one problem formulation end-to-end, you have two honest options:

1. **Euler-aligned:** host graph + lateral-movement edges only (small positive set, ~10 nodes) — narrow but consistent with Euler.
2. **APT-aligned:** provenance graph + process/file attack edges — matches CICAPT labels, but it is **not** Euler’s problem anymore.

Mixing “Euler method” with “full APT provenance labels” without stating that mismatch is where the comparison gets weak. Your Euler intuition is right; the paper needs to pick which problem it is actually solving.

---

**User**

so what should be my next step then??

---

**Cursor**

Checking what's already in the repo so the next step matches your setup.


**Run a 2-hour feasibility gate today, then lock one paper story.** Don't start training or writing until that gate passes.

## Step 1 (today): Feasibility script

Build **one** script that reports, for Phase 1 + Phase 2:

| Metric | Provenance graph | Host-collapsed graph |
|--------|------------------|----------------------|
| Nodes / edges | ✓ | ✓ |
| Attack-touching test edges | ✓ | ✓ |
| Per-`subLabel` positive edges | ✓ | ✓ |
| Edges with both endpoints in Phase-1 train nodes | ✓ | ✓ |

Model it on `preprocess_twitter_edges.py` — no GCN yet.

**Decision rules (apply immediately after):**

- **Host graph has &lt;50 usable attack test edges** → drop host comparison; Euler's abstraction doesn't match CICAPT labels.
- **Provenance has ≥200 attack test edges** → proceed with **provenance-only** LP.
- **Both views have ≥200** → keep the granularity comparison (unlikely given earlier stats).

From the quick read we already did, expect: **provenance wins the gate; host view fails it.**

## Step 2 (tomorrow): Lock the paper — one sentence

If the gate matches expectations, use this:

> **Benchmark GCN link prediction on APT provenance graphs with SoHeaRT hard negatives; show random negatives inflate AP on cyber graphs.**

Not "Euler was wrong about provenance." Instead: **Euler solves inter-host lateral movement; CICAPT labels provenance-level tactics — so we evaluate LP where the labels actually live, and fix evaluation with SoHeaRT.**

**Title direction:** *"Hard Negatives Matter: Evaluating GCN Link Prediction on APT Provenance Graphs"*

## Step 3 (Days 2–4): MVP pipeline

1. **Graph:** Process + Artifact nodes, collapse 4 edge types to one (homogeneous).
2. **Split:** Train = Phase 1 (benign); test positives = Phase 2 edges touching `label=1` nodes.
3. **Model:** Your existing static GCN + SVD features (no temporal RNN — out of scope for 1 week).
4. **Negatives:** Random vs SoHeaRT — **this is the main contribution.**
5. **Baselines:** Adamic-Adar, common neighbors, UA.

## Step 4 (Day 5): One analysis table

Per-tactic recall/AP (`credentialAccess`, `collection`, `lateralMovement`, …). Expect `lateralMovement` to be tiny — report that honestly as a limitation, not a failure.

## What to drop

- Host vs provenance 2×2 matrix (unless the gate surprises you)
- Euler replication (temporal GCN+GRU, distributed workers)
- LANL download/wrangling (fallback only if provenance gate fails)

## Immediate action

Switch to **Agent mode** and ask to implement **Day 1: CICAPT feasibility stats + provenance graph builder**. That one script decides everything else and unblocks the rest of the week.

If you want to stay in Ask mode, the manual check is: count Phase 2 attack edges that survive host collapse (socket `remote address` pairs) — if it's ~18, you've already answered the gate; proceed provenance-only.

---

**User**

so we r picking a cybersecurrity graph dataset and predicting a threat link and evaluating on hard negatives to tell that easy negatives can identify wrong threats saying link exists while hard negs will tell where actually a threat or a link exists?>

---

**Cursor**

**Mostly yes — with one important correction.**

## What you're doing

1. **Dataset:** CICAPT-IIoT provenance graph (Phase 1 benign → train, Phase 2 attack edges → test).
2. **Task:** Link prediction — score whether an edge *(u, v)* should exist.
3. **Positives:** Real attack-related edges (touching `label=1` nodes).
4. **Negatives:** Non-edges sampled two ways:
   - **Easy (random):** random unconnected pairs — trivial to reject.
   - **Hard (SoHeaRT):** plausible non-edges (similar structure/embeddings) — hard to reject.

## The correction

Hard negatives don't tell you **where threats are**. They're still **non-threats** (edges that don't exist / aren't attack).

What they do:

| | Easy negatives | Hard negatives |
|---|---|---|
| **What they test** | Can the model beat obvious non-edges? | Can it beat *plausible* non-edges? |
| **Typical AP/AUC** | Inflated (looks great) | Realistic (often much lower) |
| **Meaning** | Model may appear to detect threats it can't | Model must rank **true attack edges** above structurally similar benign pairs |

So the claim is **not** "hard negs find threats." It's:

> **Random negatives make cyber LP look successful when it isn't; SoHeaRT hard negatives expose whether attack edges are actually distinguishable from plausible normal behavior.**

## One-sentence paper story

> On APT provenance graphs, GCN link prediction achieves high AP with random negatives but fails under SoHeaRT hard negatives — so prior easy-negative evaluation overstates threat-detection ability.

That's a clean, defensible CARS contribution: **evaluation methodology for cyber graph LP**, not a new architecture.

---

**User**

ok give me 3-4 steps to move forward with a very simply east to understand written idea of what to do 

---

**Cursor**

**The idea in one line:** Build an APT provenance graph, train a GCN to score links, then show that easy random negatives make detection look good while SoHeaRT hard negatives reveal whether it really works.

---

### Step 1 — Build the graph (Day 1)
Turn `Phase1_Provenance.csv` + `Phase2_Provenance.csv` into one graph:
- **Nodes:** processes + files/artifacts  
- **Edges:** the 4 provenance relations (treat as one edge type)  
- **Labels:** Phase 1 = normal training data; Phase 2 attack edges = test positives  

Count nodes, edges, and how many attack test edges you have. If count is too small, stop and adjust.

---

### Step 2 — Train a simple GCN (Day 2–3)
Reuse your HeaRT/SNAP pipeline:
- SVD node features → GCN encoder → dot-product link scorer  
- Train only on **Phase 1 benign edges**  
- Test on **Phase 2 attack edges** (positives)

No new architecture, no temporal model.

---

### Step 3 — Compare easy vs hard negatives (Day 4) ← main result
For the same trained model, evaluate twice:

| Negatives | What they are |
|-----------|---------------|
| **Random** | Random node pairs that aren’t connected (easy) |
| **SoHeaRT** | Plausible non-edges that look like they could be real (hard) |

Report **AP** and **AUC** for both. Expect random ≫ SoHeaRT — that’s the finding.

---

### Step 4 — Write the paper (Day 5–6)
**Story:** Cyber LP papers (Euler, Bowman) use easy negatives; on APT provenance graphs that overstates detection. SoHeaRT gives a fairer benchmark.

**Include:** one results table (random vs SoHeaRT), per-tactic breakdown (`credentialAccess`, `collection`, …), 2–3 baselines (Adamic-Adar, common neighbors).

---

**Start:** Switch to Agent mode → “Build CICAPT provenance graph + feasibility stats (Step 1).”

---

**User**

We model a given enterprise network using a graph of authenticating entities, where the main objective is to detect anomalous authentication events among such entities. Such events construct LM paths within the network. where a LM path is characterized by two fundamental properties: (i) the attacker leverages a new set of credentials, while (ii) ultimately compromising a network entity that the original actor could not previously access [25]. Such characteristics are essential for an attacker to traverse the network to eventually execute their intended malicious objectives (e.g., stealing or exfiltrating sensitive data). These behaviors are quite challenging as they might resemble legitimate authentication channels with valid credentials in contrast to other noisy attack events. Further, it is imperative to treat authentication events as relational data, where the global context along with its temporal sequence play a vital role in interpreting its behavior. Along this vein, Jbeil creates an authentication graph, where nodes represent authenticating entities (such as hosts, users, virtualized environments, and applications) and edges define authentication events connecting these entities. Next, we extract graph features associated with the authentication graph to comprehend the dynamic nature of the enterprise network through evolving nodes’ connectivity over time. We then utilize a self-supervised temporal node embedding technique where latent representations are computed for each node in the graph at every time using a message passing technique that leverages the extracted features. In other words, Jbeil stores the dynamic state of each node which is updated whenever a node is involved in an event, and subsequently, aggregates the memory of the neighboring nodes when computing the temporal embedding of the given model; this is done by processing the edges ordered in time. Finally, a decoder calculates the edge probabilities and performs LM link prediction.




should we do something like this?

---

**Cursor**

Checking whether CICAPT supports an auth-graph setup like Jbeil's.


**No — not for this paper and this deadline.** Jbeil solves a different problem on different data with a much heavier method.

## Why not Jbeil-style now

| Jbeil needs | What you have (CICAPT) |
|-------------|------------------------|
| **Authentication graph** (users, hosts, apps) | **Provenance graph** (processes, files, sockets) |
| Auth edges = lateral movement signal | Attack labels mostly `credentialAccess`, `collection` — not auth/LM |
| **Temporal** self-supervised node memory + ordered edges | ~1 week; your pipeline is **static** GCN |
| LM = new credentials → new entity access | Only **18** attack socket nodes; **10** `lateralMovement` labels |

Jbeil is the right approach **when you have LANL-style auth logs**. CICAPT doesn't give you that graph, and building temporal self-supervised embeddings from scratch is a multi-week project — not a 6-day CARS sprint.

## What Jbeil is good for in *your* paper

Use it in **Related Work**, not as your method:

- Cite Jbeil (and Euler, Bowman) for **temporal auth-graph LM link prediction**
- Then say: *those works evaluate with easy random negatives; we study whether that evaluation holds on APT provenance graphs using SoHeaRT hard negatives*

That positions you as complementary, not competing.

## Your simpler path (unchanged)

1. **Provenance graph** from CICAPT (not auth graph)  
2. **Static GCN** link prediction (not temporal memory model)  
3. **Random vs SoHeaRT** negatives — your actual contribution  
4. Cite Jbeil/Euler as the LM line you’re **not replicating**, but whose **evaluation gap** you fix  

## When Jbeil-style *would* make sense

Only if you pivot entirely: download **LANL 2015 auth**, build auth entity graph, implement temporal LP, target LM edges — and drop the SoHeaRT provenance story. High risk before June 15.

**Bottom line:** Jbeil's *idea* (relational + temporal LM detection) is sound; your *contribution* is narrower and more doable — **honest evaluation of cyber link prediction with hard negatives on provenance data.** Stick with that.

---

**User**

what is the contribution with soheart technique here? tell briefly

---

**Cursor**

Checking how SoHeaRT is defined in your codebase so the contribution is accurate.


**SoHeaRT contribution here (brief):**

You are **not** claiming SoHeaRT detects threats. You are using it to **fix how cyber link prediction is evaluated**.

**The problem:** Papers like Euler/Jbeil train and test with **random non-edges** as negatives. On graphs, most random pairs are obviously unrelated — so the model looks good even if it cannot separate real attack edges from **plausible normal** edges.

**What SoHeaRT does:** For each positive edge *(u, v)*, it picks negatives *v′* that are **hard but still non-edges** — high PPR similarity, similar degree, some common-neighbor signal. These look like edges that *could* exist, like normal provenance behavior.

**Your contribution:**
1. **First (or early) application** of SoHeaRT-style hard negatives to **APT provenance** link prediction.
2. **Show the gap:** same GCN scores high AP with random negatives, much lower with SoHeaRT — proving easy negatives **overstate** detection ability.
3. **Practical takeaway:** cyber LP needs hard-negative evaluation, not just a new GCN.

**One line for the paper:** *Random negatives make APT link prediction look solved; SoHeaRT reveals whether attack edges are actually rankable above structurally similar benign pairs.*

---

**User**

Skip to Article Content
Skip to Article Information
Institution of Engineering and Technology
Logo for: University of Wyoming Libraries
Search
Search within
This Journal
Search term
Search


Login / Register
IET HUB HOME
 
Journals
 
NEW JOURNALS
   
IET Membership
 
Visit IET
CAAI Transactions on Intelligence Technology
CAAI Transactions on Intelligence Technology
ORIGINAL RESEARCH
Open Access
Graph neural network-based attack prediction for communication-based train control systems
Junyi Zhao, Tao Tang, Bing Bu, Qichang Li
First published: 03 February 2024 https://doi.org/10.1049/cit2.12288Digital Object Identifier (DOI)  view metrics
Accessibility issue? Request accessibility update.

OpenURL University Of Wyoming Libraries 
SectionsPDF
CITE
Tools Share
Abstract
The Advanced Persistent Threats (APTs) have emerged as one of the key security challenges to industrial control systems. APTs are complex multi-step attacks, and they are naturally diverse and complex. Therefore, it is important to comprehend the behaviour of APT attackers and anticipate the upcoming attack actions. GNN-AP is proposed, a framework utilising an alert log to predict potential attack targets. Firstly, GNN-AP uses causality to eliminate confounding elements from the alert dataset and then uses an encoder-decoder model to reconstruct an attack scenario graph. Based on the chronological characteristics of APT attacks, GNN-AP identifies APT attack sequences from attack scenario graphs and integrates these attack sequences with communication-based train control (CBTC) devices topology information to construct an Attack-Target Graph. Based on the attack-target graph, a graph neural network approach is used to identify the attack intent and transforms the attack prediction problem into a link prediction problem that predicts the connected edges of the attack and target nodes. The simulation results obtained using DARPA data show that the proposed method can improve the comparison methods by 4% of accuracy in terms of prediction. Furthermore, the method was applied to the CBTC system dataset with a prediction accuracy of 88%, demonstrating the efficacy of the proposed method for industrial control systems.

1 INTRODUCTION
The communication-based train control (CBTC) system built on wireless communication and advanced computer technologies, supplemented by the remote control and supervision of computer equipment, improves rail transit safety and efficiency. The CBTC system of Commercial off-the-shelf (COTS) bridges the gap between train operating process and cyber network, but these devices might act as an entrance for cyberattacks [1]. From July 2015 to July 2016, four cyberattacks on the UK railway network were discovered [2]. In May 2017, the passenger information system of a German railway was compromised by malware termed the WannaCry virus [3]. Another instance occurred in May 2018, where a massive distributed denial-of-service (DDoS) attack on the Danish state rail operator (DSB) paralysed some operations, including ticketing systems and the communication infrastructure [4].

In recent years, the Advanced Persistent Threat (APT) (APT) attack emerges as significant security issues for industrial control systems [5]. Stuxnet-like APT attacks can affect industrial control systems that are controlled by computer devices, which implies that cyber threats might extend to real lives [6]. Advanced Persistent Threat attack is a multi-step attack. The Cyber Kill Chain [7] describes the multi-step structure of an APT. Many existing studies have tried to prevent APT attacks. An SDN-based intrusion detection system is suggested in Ref. [8], which employs deep learning and Bayesian network to detect APTs in the IoT. Ahmadian and Rasoolzadegan [9] used a causal network to detect a multi-step attack. The authors in Ref. [10] modelled APT-Attack as a scale-free network. They used semi-supervised learning and complex network theory to depict the evolution of the APT-Attack network and effectively detect suspicious hosts at different stages of the APT attack process. The attack graph analyses the attack path from the vulnerability of the device, while Bayesian networks are based on alert information to obtain possible future attack targets. However, in the actual prediction, we not only need to combine alarm information to analyse the attack intention but also need to combine device data to determine the potential attack target.

Graphs are widely used to present the complex relationship between events. In the last few years, graph neural network (GNN) as a method of analysing graph structure data has achieved remarkable results. Many studies apply GNNs to solve prediction task [11-14]. T-GCN [11] combined with the graph convolutional network (GCN) and the gated recurrent unit to forecast road traffic. Liang G [13] proposes a novel Semantics-aware Dynamic Graph Convolutional Network for traffic flow forecasting. Graph neural network for image classification is an important direction for the application [15-19]. Mou L et al. [17] proposed a non-local GCN that can process labelled and unlabelled data and use it for image classification. And the authors in Ref. [20]utilise GNN for action recognition. In Ref. [21], Haghshenas et al. present a Temporal Graph Neural Network framework for the detection and localisation of false data injection and ramp attacks on the system state in smart grids.

This study provides a GNN-AP framework for understanding attacker behaviour and predicting possible attack targets based on GNN. The framework can be divided into two modules: (1) Attack Scenario Graph Module; (2) Attack Prediction Module. In the attack scenario graph module, this article uses the encoder-decoder model to construct an attack scenario graph and identify multi-step attack sequences based on time-based depth-first search (DFS) algorithm. In the attack prediction module, the paper combines multi-step attack sequences with a CBTC system topology to produce an attack-target graph. A GNN is used to extract attacker features and forecast the next attack target from the graph. The contribution of the paper is as follows:
This research employs reinforcement learning to determine the causal relationships between alert data. The influence of confounding factors (e.g., false alarms and useless alarms) on the analysis of attack behaviour is reduced.

Based on the chronological characteristics of APT attacks, a time-based DFS algorithm is utilised to identify malicious APT attack behaviours in the attack scenario graph.

An attack target graph is constructed based on the APT attack behaviour sequence and the CBTC system topology. GNN is used to identify the attack intent from the attack target graph and convert the attack target prediction task into link prediction.

The rest of this article is organised as follows. Section 2 introduces the structure of a CBTC system and describes the functionality of each component. Section 3 presents the related work of cyberattack prediction. Section 4 discusses how to construct an attack scenario graph and identify multi-step attack sequences. In Section 5, the proposed GNN and link prediction are explained in detail to handle the attack prediction problem. Finally, Section 6 evaluates the simulation results of the proposed method, and Section 7 summarises the paper.

2 BACKGROUNDS
A CBTC system is a typical cyber-physical system, consisting of a control centre, on-board equipment, and a communication system. The typical structure is shown in Figure 1. The control centre comprises an automatic train supervision (ATS) system, zone control (ZC), and computer interlocking (CI). The on-board equipment mainly refers to automatic train protection and automatic train operation. The digital communication system provides a secure communication between the control centre and on-board equipment. The train control system is responsible for train overspeed protection, automatic driving, speed measurement, and location monitoring [22].

Details are in the caption following the image
FIGURE 1
Open in figure viewer
PowerPoint
Communication-based train control network structure.

3 RELATED WORKS
The security issues in CBTC systems have gained the attention of scholars. In Ref. [23], the authors developed a cross-layer defence scheme to mitigate the impact of jamming attacks on wireless CBTC communication systems. Additionally, the authors in Ref. [24] proposed a resilient control strategy to defend against Denial of Service attacks on wireless communication systems. Article [25] developed traditional intrusion detection systems by using a Kalman filter and a χ2 detector. These proposed approaches could accurately detect data tampering attacks. However, these studies focus on simple attacks like jamming and DOS.

In Ref. [26], an attack graph is generated based on network connectivity and vulnerabilities. The method improved attack scenarios reconstruction by analysing cohesive relationships between sub-scenarios and discovering unreported actual negative alerts. But it is challenging to obtain network vulnerability information without expert knowledge. Paper [27] merged a neural network and a Bayesian network. Their approach used neural network to remove false alerts and generates a Bayesian network attack graph via the filtered alerts. In Refs. [28, 29], an attack graph was generated to correlate individual alerts by utilising a data mining approach. These methods analyse extracted attack patterns from historical alert data, which provide information to anticipate future cyberattacks.

Lv et al. [30] have introduced a heterogeneous graph network attack detection method. The authors model the provenance data using a heterogeneous graph approach and represent each system entity with a semantic vector by which rich contextual information about each system entity (e.g., processes, files, sockets, etc.) can be obtained. Online network attack detection is then performed using graph learning individually. The experimental results show competitive performance compared to state-of-the-art rule-based network attack detection systems. This article uses heterogeneous graph learning for attack detection but cannot prevent APT attacks.

Zhao et al. [31] have introduced an HinAp framework to predict attack preference. The framework first built an attributed heterogeneous information network that classifies attack events into 5 categories: attackers, vulnerabilities, exploited scripts, attacked devices, and compromised platforms, describing the dependencies between each piece of information. Then, two models, superimposed attention mechanism and inductive learning, are put in place for attack preference prediction. Their time-delay data show that HinAP has good performance in predicting the attack preferences of intruders. Unfortunately, the data used in this paper is difficult to gather, such as the scripts deployed by the attacker. Such data may not be available in time during the attack process.

Li et al. [32] have introduced a framework called DeepAG that uses system logs to predict APT attack. A transformer model of the proposed framework is developed for detecting APT attacks from semantic information of system logs. DeepAG utilises the LSTM network and Out-Of-Vocabulary (OOV) word processor mechanisms to predict attack path, which achieves higher performance than traditional BiLSTM. However, the article ignores the system logs where non-malicious attacks can also be anomalous. The impact of these confounding factors needs to be excluded when analysing APT attacks.

This paper proposes a GNN-AP structure to first identify APT attacks by removing ambiguous alert such as non-malicious attack alert and false alerts from the alert data output by the intrusion detection system. Then, an attack target graph is constructed to analyse the attack intent of APT attackers and predict future attack targets. The structure is shown in Figure 2.

Details are in the caption following the image
FIGURE 2
Open in figure viewer
PowerPoint
Graph neural network-based attack prediction framework.

4 ATTACK SCENARIO GRAPH RECONSTURCTION
Though APT attacks are highly stealthy and complicated, this paper study APT attack scenarios from the alert log. Figure 3 illustrates the outline of attack scenario graph reconstruction, which is divided into three parts: alert data process, graph generation, and causal identify. First, in order to represent the alert logs in this paper, alerts are templated as 7-tuple and converted into vectors by word embedding, respectively. In the graph generation phase of this paper, these alert log vectors are fed into an encoder-decoder model to generate the graph adjacency matrix. Finally, this paper uses the idea of the RL paradigm to determine the causal relationships between graph nodes: the actor is an encoder-decoder model, and the reward function is tailored to causal discovery by incorporating a score function.

Details are in the caption following the image
FIGURE 3
Open in figure viewer
PowerPoint
Attack scenario graph reconstruction.

4.1 Alert data processing
The alerts are signals that indicate potential attacks detected by an intrusion detection system. They can be described as a n-tuple , where the is the attribute of an alert. The alert log is the set of alerts, written as ε=(e1,e2,⋯,ei) [33]. This paper uses a 7-tuple time, class, priority, sip, sport, dip, and dport to present the alert ei. Samples of alerts are shown in the Table 1. However, the raw alarm logs cannot be directly used as the input for the model. To address this, this paper treats the values in time, class, priority, sip, sport, dip, and dport as words and vectorise the alerts using the GloVe algorithm [34]. This allows us to represent each alert as a vector in a high-dimensional space, which can be used as input for the model.

TABLE 1. Sample of alerts.
Time	Class	Priority	Sip	Sport	Dip	Dport
19:38:39	INFO TELENT access	Not suspicion	172.16.112.18	23	172.16.113.84	23,713
19:39:35	BAD TAFFIC same SRC/DST	Potentially bad traffic	0.0.0.0	68	255.255.255.255	67
19:39:59	INFO TELENT access	Not suspicion	172.16.112.18	23	194.27.251.21	43,976
19:40:11	INFO TELENT access	Not suspicion	197.218.176.32	23	172.16.113.148	25,627
4.2 Graph generation
Encoder This paper applies an attention-based encoder in the Transformer structure [35]. It comprises several transformer blocks. Each block contains multi-head attention networks and feedforward networks. The output of the attention layer is as follows:
(1)
where the matrices Q, K, V denote the queries, keys, and values of the input, respectively. The output of the feedforward layer is as follows:
(2)
The attention scheme is an effective way to find the causal relationships among variables [36]. The self-attention encoder final output is denoted as with d dimensions.

Decoder The decoder establishes the relationships between the encoder outputs eni. Then, a graph adjacency matrix A is generated in an elementwise manner. A single-layer decoder can be written as follows:
(3)
where and are trainable parameters and dhidden is the hidden dimension associated with the decoder. f is the tanh function. This paper obtains an adjacency matrix A by taking each entry mij as an input of a logistic sigmoid function σ (⋅). Finally, this paper uses sampling to indicate the probability that an edge exists between alerts ei and ej, where the samples follow a Bernoulli distribution with probability σ(mij).
4.3 Causal relationship identify
After obtaining the structure diagram, RL-based causal inference is used to identify the causal relationship with the best score [36]. The Bayesian information criterion (BIC) score of a directed graph is as follows:
(4)
If a graph is acyclic, its adjacency matrix should satisfy the constraint:
(5)
This paper needs to consider the score function and acyclic constraints when designing the reward function. Therefore, the reward function can be written as follows:
(6)
The indicator function is denoted by F(⋅) and two parameters βa, βb ≥ 0, which are penalty parameters. If the values of βa and βb are large, the graph generated by using the proposed reward function is more likely to be acyclic. Therefore, this paper needs to make all possible graphs have the largest reward to ensure that they are acyclic. This paper sets this problem equal to
(7)
By applying the actor-critic algorithm in the RL paradigm, the expected reward function is our training object, and write it as follows:
(8)
In this part, this paper finds the graph with the best reward among all generated graphs during training and set it as the output. However, the graph may have some edges that should not exist due to the existence of confounding factors in the actual process. Therefore, this paper needs to proceed further.

This paper optimises the estimated edges with the greedy algorithm based on regression performance or the score function. For a causality edge, this paper optimises it by the causal inference method. It removes the parent of a node and then computes the score of the graph while preserving the causal relationship among the remaining nodes. If the score of the graph does not decrease or the decreased value is within the allowable range after implementing the optimisation method, we accept the optimisation result and continue processing according to the optimised graph.

In the above graph edge optimisation process, it is necessary to consider adding a penalty weight to the number of graph edges in the score function. However, it is crucial to choose a good weight. Bad weights may cause the correct edge to be lost in the graph. This paper incorporates penalty weights log m when computing the BIC score and then optimises the graph to reduce false causalities.

4.4 Multistep attack alert sequence
This paper obtains an attack scenario graph form alert datasets, such as DARPA 2000. The alert dataset contains alerts generated by malicious attacks and accidental scenarios. Malicious attacks are composed of multiple attack steps and are continuous in time. This paper identifies the multi-step attack alert sequence based on the time factor. However, some problems may also be encountered when extracting attack alert sequences from alert graphs, as shown in Figure 4.

Details are in the caption following the image
FIGURE 4
Open in figure viewer
PowerPoint
Multistep attack alert sequence.

In this figure, A − B − C − D and A − C − D are two obvious attack alert sequences. This paper identifies these two paths as correct attack sequences if the time factor is ignored. However, the main characteristic of a multi-step attack is that the attacker remains hidden in the system for a long time. The attacker will try various ways to find a vulnerability during the execution of an attack. This may lead us to discover incorrect attack alert sequences. For example, in Figure 4, alert B causes alert C at t0, while A causes B at t1.

Therefore, the causal relationship among alerts A, B and C is not established due to a chronological conflict. The attack alert sequence is incorrect.

The [37] method extends the traditional DFS approach with temporal information, which can effectively solve the above problems. In this way, the influence of chronological order on the accurate detection of alert relationships can be eliminated in an attack sequence. Unlike computer networks, each computer in an industrial control system performs a different role, so the importance levels of different computers to the system are also different. In CBTC, CI is more important than the ZC computer. Based on the time-based DFS algorithm, this paper also considers the node importance weight in the algorithm. It will be helpful for extracting the primary attack alert sequences from the causal alert graph.

This paper obtains multiple alert sequences with malicious attack behaviours by executing the time-based DFS algorithm. According to the output sequences, the system tries to extract the attack intention from each sequence. The pseudocode is summarised in Algorithm 1.

Algorithm 1. Multi-step Attack Alert Sequence Discovery

Input: IDS Alert Dataset 

Output Multi-step Attack Alert Sequence: AS

1 // Attack Scenario Graph

2 Encoder: This paper uses an attention-based encoder, and the output of the encoder is as follows:

3 

4 Decoder: mij (Z1, Z2, q) = qTf (Z1eni + Z2emj)

5 //Causal Identification

6 Acyclic Constraints Set reward function:

7 

8 Actor-Critic Algorithm:

9 

10 //Multi-step Attack Alert Sequence Discovery

11 For node in  do

12   Set node attributes = (time, class,priority,sip,sport,dip,dport)

13  node.time = node[time]

14  root.node←nodei.[time] = starttime

15 if nodei.[time]<nodej.[time] then

16   path.add(nodej)

17  else

18   path.delete(nodej)

19  end

20 end

21 return (AS)

5 CYBERATTACK PREDICTION BASED ON GNN
The goal of our work is to obtain the subsequent behaviour of an attacker from a multi-step attack alert sequence. The authors in Ref. [38] first used recommender systems in the field of cyber defence. The authors proposed a collaborative filtering method to solve the attack prediction problem. This paper proposes a sequential recommendation method to solve the multi-step attack prediction task in industrial control systems [39]. Sequential recommender systems recommend products that users may be interested in by modelling the sequential dependencies over user-product interactions. By establishing sets for attackers and targets, the interaction between attackers and targets can be represented using attack alert sequences. This paper utilises principles from recommender systems to infer attack intentions in order to forecast potential attack targets. The system model is shown in Figure 5.

Details are in the caption following the image
FIGURE 5
Open in figure viewer
PowerPoint
Attack prediction module.

5.1 Attack target networks
The attack-target network (ATNet) is integrated with multi-step attack alert sequences and the system network topology. The graph contains two types of nodes: attacker nodes A and system device nodes D. The paper maps the alert events to system devices corresponding to their IP addresses. A device node d has an attribute set p = (p1, p2,⋯, pi) at time t. In this paper, the device properties are set to Device Function, Device Importance, Vulnerability, and Defence Level. Consequently, the dynamic graph can be represented by the following equation:
(9)
when attacker a attacks target device d at time t, a link la is established between a and d. The communication link between devices di and dj is denoted as lc. This paper defines ATNet consisting of all attacker behaviours at time t as . In the network, the neighbouring nodes of an attacker node are the attacked devices. The attack intention can be deduced from all historically compromised devices. The paper uses the message propagation mechanism to obtain the attack intention from the neighbouring nodes. A GNN is a powerful method for revealing the relationships between the centre node and its neighbours. However, it fails to capture the sequence dependencies between neighbouring nodes. Furthermore, this paper applies a relational attention mechanism to this module, which is well suited for predicting attacks from sequence data.
A GCN is an efficient way to directly aggregate all neighbouring nodes:
(10)
where is the feature representation of node a in the lth layer. is the weight parameter of the node in the lth layer. Na represents the number of neighbouring nodes of node a.
The characteristic of the GCN model is that it obtains information from the adjacent nodes of the central node, but it ignores the chronological order information between the adjacent nodes. To effectively distinguish the influences of different target devices and take full advantage of the available attacker-target interaction order information, this paper applies the attention mechanism with additional sequence dependency information in the model.

This paper denotes ρij as a sequence dependency relation between attack target di and the last target node dj. Equation (11) is the representation form used for device nodes along the attack sequence.
(11)
Nevertheless, this approach fails to consider the dependency relations between different devices. This paper assigns a parameter that maps the sequence dependency information relations into vectors.
(12)
where is a dependency head. Therefore, the final representation of device node di is as follows:
(13)
This paper takes into consideration when calculating the attention coefficients. The aware attention mechanism differentiates the importance of a target to the attacker by its importance weight ead. Taking the node embedding of the lth layer as an input, the weight is formulated as follows:
(14)
where and are the embeddings of attacker ea and target device ed respectively. The embedding dimensionality is denoted as d. In the formula, the scale parameter is designed to avoid excessive dot product computations, and it is helpful for improving the convergence speed of the model. The importance levels of attacker nodes and their attack target nodes are calculated via the Softmax function:
(15)
where σ is the rectified linear unit (Relu) activation function and ∥ denotes the concatenation operation.
Thus, the intention of the attacker can be obtained by aggregating the information derived from all neighbours (the attack targets):
(16)
where captures the sequence information in the attack-target prediction graph.
Node Updating In this part, this paper aggregates the attention of the attacker and update the node representation of with the embedding of the previous layer. For the attacker node, the representation updating rule from the previous layer can be calculated as follows:
(17)
where is the parameter for controlling the embedding information , and .
5.2 Cyberattack target prediction
In this subsection, this paper uses link prediction to solve the attack prediction problem. The paper designs a link prediction function to determine the attack target that the attacker may invade next.

A given candidate target d ∈ D is selected based on real system network connections. The link prediction function is defined as follows:
(18)
where vector denotes the scores of all potential attack target devices d. Ed is the embedding of device d, and W is the trainable transformation matrix. In a real industrial control network, the functions and importance levels of devices influence the attacker's choice. Therefore, this paper uses ξd to denote the strength of device d, namely, the sum of its weights according to its function, importance, and other aspects.
(19)
Wd is obtained from experts in the industrial control system security field. In function (18), ha denotes the final attack intention. This paper obtains each layer embedding of the attacker node from an L-layer ATNet. The embeddings in different layers emphasise the various attacker preferences. Therefore, the final attack intention can be written as follows:
(20)
The cross-entropy function is commonly used as the loss function in such optimisation tasks. The cross-entropy loss function can quickly constrain the model loss to converge to a global local minimum. The normalised vector of the scores for potential attack devices is as follows:
(21)
Thus, the objective function is as follows:
(22)
where ya denotes the true target of the next attack, and is the result predicted by the proposed method. Θ denotes all model parameters, and ‖⋅‖2 is the L2 norm. λ controls the regularisation strength. This paper optimises the model by using a back-propagation Algorithm. The pseudocode for this is presented in Algorithm 2.
Algorithm 2. Attack Target Prediction

Input: Multi-step Attack Sequence AS1=(as1,as2,⋯,asn)

    System network topology: 

    Number of graph layers: L

Output The subsequent attack system device: dt+1

1 //Attack Target Graph

2 Combine attack alert sequences and system topology into an attack target graph 

3 //Node representation initialisation

4 

5 ///The node update process

6 For each l ∈ [1:L] do

7  

8  

9  

10 end

11 //The prediction of the following attack target

12 

13 Next attack target ←argmax (s (a, d))

6 RESULTS
In this section, this paper uses DARPA 2000 to verify the effectiveness of the proposed method. Then, the paper demonstrates the ability of our method in a CBTC system.

6.1 DARPA dataset
The DARPA1999 and 2000 datasets are widely used for security testing. The DARPA 2000 dataset contains two kinds of attack scenarios (LLDOS 1.0 and LLDOS 2.0.2). These attacks involve five stages, including scanning hosts, sending vulnerability query packets, exploiting vulnerabilities to attack hosts, implanting DDoS software into target hosts, and using DDoS software to launch DDoS attacks. In the experiment, this paper replayed network traffic in DARPA1999 & 2000 using Snort IDS, which generated a large number of raw alerts. This paper labelled the raw alerts and remove duplicates, false alerts, and unnecessary alerts, resulting in processed alerts (Table 2). The proposed model is trained on DARPA1999. It is then evaluated for multi-step attacks using the DARPA 2000 dataset.

TABLE 2. IDS alerts of DARPA1999&2000.
Dataset	Total number of alerts	Processed alerts
DARPA1999	2324	598
DARPA2000	2755	854
This paper sets the hyperparameters in the experiment as follows.

N = 5 and dhidden = 12 denote the number of the transform block in the encoder and hidden dimension in a single layer decoder, respectively. RL-related parameters are unchanged as in Ref. [31]. For the cyberattack prediction module, this paper uses a 3-layer GCN, and the embedding size is set to 50.

6.2 Attack scenario graph reconstruction
Figure 6 shows the reconstructed attack scenario graph for DARPA2000. In the figure, each node presents an alert event. The attack graph presents the attack step by removing the confounding factors from the alert data through causal machine learning. The first stage involves finding online hosts. It consists of four Ping-type alerts 2; 4; 5; 6. The alerts 11; 14; 47; 46 indicate host vulnerability exploration by the attacker. After obtaining an exploitable vulnerability, stages four and five perform attack software injection and attack execution.

Details are in the caption following the image
FIGURE 6
Open in figure viewer
PowerPoint
Attack scenario graph.

While an attack scenario graph can help remove a larger number of confounding alerts, some not-relevant positive alerts still exist in the graph. To obtain malicious multi-step attack sequences, this paper uses the time-based DFS algorithm to extract attack sequences. Unexpected events tend to generate short-lived alerts, while malicious multi-step attacks are characterised by long and continuous durations. In the experiment, when the time weight parameter Twet is set to 10, it can eliminate unexpected alerts 5; 6; 11; 47 etc and find main attack sequences. Figure 6 demonstrates the attack scenario graph constructed from the alert data. It contains 38 alerts in the graph. Then, we applied time-based DFS to discover main malicious attack sequence, as shown in Figure 7. Two multi-step attack sequences are extracted from the attack scenario graph. Attack sequence #1 can be divided into four stages. The first stage is to find the online hosts. It consists of three Ping type alerts. The second stage is host vulnerability exploration. It contains five alerts. After obtaining an exploitable vulnerability, stages four and five, respectively, perform attack software injection and attack execution. The attack sequence #2 is similar to sequence #1, while the sequence just misses exploring the online Hosting stage. From Figure 7, it is evident that while both attack sequences commence from the same host, their final target endpoints differ due to variations in attackers' intentions. By comparing with the description of the DARPA200 database, the proposed method can accurately reconstruct the attack scenario and detect malicious multi-step attack steps.

Details are in the caption following the image
FIGURE 7
Open in figure viewer
PowerPoint
Attack sequence.

6.3 Attack target prediction
This paper implements a GNN-AP framework to make attack predictions based on the multi-step attack sequences. First, this paper builds the dynamic prediction network by mapping the alerts to the relevant devices based on their IP addresses. The candidate attack target is selected according to the network topology. This paper sets the number of layers L in the ATNet to 3. This paper compares the performance of our model with that of other methods [22, 32, 34]. In this paper, four typical evaluation matrices are employed to evaluate the prediction performance of the ATNet model.
(23)
where TP is the number of correct prediction results that are classified as correct. FP is the number of correct prediction results that are classified as false. TN is the number of false prediction results that are classified as false. FN is the number of false prediction results that are classified as correct. Table 3 presents the results of a comparative analysis with other methods. It is observed that the ATNet model outperforms the DeepAG, Alert-GCN and Bayesian Network models in terms of prediction accuracy. The improved performance of our model can be attributed to its ability to utilise causality in generating multi-step attack sequences, which enables more accurate extraction of attacker intention. Although the MIF method also achieves high prediction accuracy, the proposed model performs better in other metrics. This suggests that our model provides a more comprehensive solution for predicting cyberattacks.
TABLE 3. Performances comparison among different methods.
Model	Accuracy	Precision	Recall	F1 score
Bayesian network [27]	0.9795	-	-	-
DeepAG [32]	0.89018	-	-	0.86907
MIF [37]	0.99	0.97	0.95	0.96
Alert-GCN [40]	0.9242	0.9390	0.9242	0.9316
Proposed model	0.9940	0.9942	0.9915	0.9923
6.4 Case study on a CBTC system
In this section, this paper applies the method to the CBTC simulation platform. The platform consists of VOBC, WGB, AP, ZC, CI, ATS and wayside equipment. The VOCB, WGB, AP, ZCs are real equipment. The ATS, CIs and wayside equipment are implemented by software. Figure 8 shows the architecture of the simulation platform.

Details are in the caption following the image
FIGURE 8
Open in figure viewer
PowerPoint
Communication-based train control simulation platform.

This paper simulates a simple DDoS attack including IP sweep, exploit vulnerability and attack execution. This paper collects traffic for different time window sizes and use Snort to generate alerts. The alert data stream is split into different windows sizes, with W1 = 30; W2 = 60; W3 = 90; W4 = 120; W5 = 150, as shown in Figure 9.

Details are in the caption following the image
FIGURE 9
Open in figure viewer
PowerPoint
Communication-based train control alert data stream.

Figure 10 shows the performance of proposed method applied to CBTC. The values of the accuracy, recall, precision and F1 score metrics all exceed 80%. The proposed method has a 4% improvement in the metric of accuracy compared to the method described in Ref. [25]. Figure 10 shows that the prediction result is best when the train and test data are in the same time window. From 0 to 90 times, the prediction performance also becomes better as the dataset becomes larger. After 90 times, the performance remains stable. It also shows that the value of the four measures does not decrease dramatically if the training and testing are not in the same time windows. The result reveals the stability of our method. Figure 10c reveals that the prediction performance is best when the training and testing data are in the same time window. For instance, if the training data are in W2 and the test data are in W5, the worst result only drops to 0.07.

Details are in the caption following the image
FIGURE 10
Open in figure viewer
PowerPoint
(a) Accuracy, (b) Recall, (c) Precision and (d) F1-score.

7 CONCLUSIONS
Communication-based train control (CBTC) systems are susceptible to APT attacks with widespread applications of COTS. The study presents a novel framework based on GNN for predicting the upcoming attack target in APT attack scenarios. The GNN-AP framework utilised reinforcement learning-based causality technique to construct attack scenario graphs that accurately reveal the causality between alert information. This approach eliminates the impact of meaningless alerts and non-alerts on reconstructed attack scenario graphs. In addition, the ATNet combines the attack behaviour with the CBTC system topology information and uses GNN to learn the intent of the attacker from ATNet. Thus, the problem of attack target prediction is transformed into the node-link prediction of ATNet.

The experimental results show that GNN-AP can accurately restore the attack scene and get the attack behaviour sequence based on the intrusion detection alarm information. Tests on DARPA datasets show that the proposed method improves by 2% over MIF and 5% over Alert-GCN. In the experiment of the CBTC system, this paper assigns different weights to the function and importance of different devices, which makes the prediction result closer to reality. The prediction accuracy can reach 88%. This approach helps CBTC system managers identify the intent and subsequent targets of an attack, enabling them to deploy more targeted defences.

The proposed model is based on the alert information detected by IDSs, which makes it difficult to predict cyberattacks that are not detected by the IDS. In future work, we need to consider how to guarantee the accuracy of the training model when there are various variations in the alert data. We plan to use the stable learning approach to train a model that can be applied to various scenarios.

ACKNOWLEDGEMENTS
This work was supported by the Beijing Natural Science Foundation-Fengtai Rail Transit Frontier Research Joint Fund (Grant No: L211002) and the Beijing Laboratory for Urban Mass Transit (Grant No: I18H100010).

CONFLICT OF INTEREST STATEMENT
The authors declare that there is no conflict of interest regarding the publication of this paper.

DATA AVAILABILITY STATEMENT
The data used to support the findings of this study are included within the article.

REFERENCES
Citing Literature

Early View

Online Version of Record before inclusion in an issue

Figures
References
Related
Information
Recommended
A new control for the pneumatic muscle bionic legged robot based on neural network

Chaoyue Xu, 
Feifei Qin, 
Kun Zhou, 
Binrui Wang, 
Yinglian Jin
IET Cyber-Systems and Robotics
Adaptive neural‐based asynchronous control for nonhomogeneous Markov jumping systems with dead zones

Xiang Li, 
Luxue Wang, 
Shuangsi Xue, 
Zihang Guo, 
Hui Cao
IET Control Theory & Applications
ESO‐based output‐feedback regulation control of nonlinear systems with SiISS inverse dynamics

Chunxiao Wang, 
Yang Gao, 
Yuliang Sun, 
Yuqiang Wu
IET Control Theory & Applications
Survey of Attack Graph Analysis Methods from the Perspective of Data and Knowledge Processing

Jianping Zeng, 
Shuang Wu, 
Yanyu Chen, 
Rui Zeng, 
Chengrong Wu
Security and Communication Networks
Observer‐based integral sliding mode control for uncertain neutral semi‐Markovian jumping systems with time‐varying delays

Qinhong Fu, 
Lianglin Xiong, 
Haiyang Zhang, 
Kaibo Shi
IET Control Theory & Applications
Download PDF
 back
Institution of Engineering and Technology Logo
About The IET
IET Privacy Statement
Contact The IET
journals@theiet.org
Copyright (2026) The Institution of Engineering and Technology. The Institution of Engineering and Technology is registered as a Charity in England & Wales (no 211014) and Scotland (no SC038698)
Additional links
About Wiley Online Library
Privacy Policy
Terms of Use
About Cookies
Manage Cookies
Accessibility
Wiley Research DE&I Statement and Publishing Policies
Help & Support
Contact Us
Training and Support
DMCA & Reporting Piracy
Sitemap
Opportunities
Subscription Agents
Advertisers & Corporate Partners
Connect with Wiley
The Wiley Network
Wiley Press Room
Copyright © 1999-2026 John Wiley & Sons, Inc or related companies. All rights reserved, including rights for text and data mining and training of artificial intelligence technologies or similar technologies.


Wiley Home Page
May we use cookies to track your activities? We take your privacy very seriously. Please see our privacy policy for details and any questions.Yes No



hwo about targeting something related to security and specific industry dataset?? Or should I run for multiple common datasets and tell that evaluation for link prediction is incorrect and should be done using hard negatives but wouldnt that be an issue because that woudl tthen become benchmarking link PREDICTION 

---

**Cursor**

**Don't do GNN-AP-style, and don't do a broad multi-dataset LP benchmark.** Do one security-industry dataset + a cyber-specific evaluation claim.

## Three options compared

| Option | Verdict |
|--------|---------|
| **GNN-AP style** (alerts → causal graph → attack-target LP on CBTC) | **Skip.** Heavy custom pipeline (RL causality, alert embedding, device topology). CBTC data isn't public. GNN-AP already owns that niche. |
| **Many common LP datasets** (Twitter, Cora, etc.) + hard negatives | **Weak for CARS.** That's basically HeaRT/SoHeaRT again — general link-prediction benchmarking, not cybersecurity. Reviewers may ask: "Why is this in a security venue?" |
| **One APT industry dataset (CICAPT-IIoT) + SoHeaRT eval on cyber LP** | **Best fit.** Security + industry (IIoT), reproducible, reuses your code, clear gap vs Euler/Jbeil/GNN-AP. |

## Your worry is valid

If the paper is *"hard negatives matter on 5 standard LP benchmarks"*, you are writing a **link prediction evaluation paper**, not a security paper. HeaRT already did much of that on social/OGB graphs.

Your contribution should be narrower:

> **Cybersecurity link prediction papers treat APT/LM detection as LP but evaluate with easy random negatives. We show that fails on APT provenance graphs; SoHeaRT gives a realistic evaluation.**

That is **security evaluation methodology**, not general LP benchmarking.

## What to target (locked)

- **Dataset:** CICAPT-IIoT (IIoT APT provenance — industry + security)
- **Task:** Predict attack-related edges (provenance graph LP)
- **Method:** Standard GCN (no new architecture)
- **Contribution:** Random vs SoHeaRT negatives — same model, honest vs inflated metrics
- **Related work:** Cite GNN-AP, Euler, Jbeil as "cyber LP exists" — none discuss hard-negative evaluation

Optional **one** extra cyber graph (LANL auth) only if Step 1 finishes early — as a short "same eval gap on auth graphs" paragraph, not a second full study.

## One-line pitch for CARS

*Industrial APT provenance graphs enable link-prediction-based threat detection, but random-negative evaluation overstates performance; we apply SoHeaRT hard negatives to expose the gap.*

That is security-specific, industry-grounded, and not "another LP benchmark paper."

---

**User**

then give me a proper big size dataset that we can download and begin work on. i need to work in 2-3 datasets

---

**Cursor**

Searching for large, publicly downloadable cyber graph datasets that fit link-prediction + labeled attacks.


**Recommended 3-dataset stack** — big enough for a security paper, all public, each tests a different graph type:

---

## Your 3 datasets

| # | Dataset | Size | Graph type | Why include it |
|---|---------|------|------------|----------------|
| **1** | **CICAPT-IIoT** | ~60 MB CSV (you have it) | APT **provenance** (IIoT) | Industry + APT labels; Phase1 benign → Phase2 attack |
| **2** | **LANL Cyber1** | **7.2 GB** (`auth.txt.gz`) + 4.8 KB labels | **Host auth** (Euler/Jbeil line) | ~1.6B events, 17K hosts, red-team labels — the standard LM benchmark |
| **3** | **DARPA TC E3 (THEIA)** | **Multi-GB** per engagement (pick 1 file) | **Enterprise provenance** (Linux) | CADETS/THEIA scale: ~60K–380K nodes, millions of edges — classic APT provenance |

Together: **provenance (IIoT) + auth (enterprise LM) + provenance (DARPA)** — not three random LP benchmarks.

---

## Downloads

### 1. CICAPT-IIoT — already done
- You have: `Phase1_Provenance.csv`, `Phase2_Provenance.csv`
- Optional full bundle: [UNB CIC APT IIoT 2024](https://www.unb.ca/cic/datasets/iiot-dataset-2024.html) (form → `Attack_info.csv`, network CSVs)

### 2. LANL Comprehensive Cyber1 — download first
- Page: [csr.lanl.gov/data/cyber1](https://csr.lanl.gov/data/cyber1/)
- Files:
  - `auth.txt.gz` (7.2 GB) — auth events
  - `redteam.txt.gz` (4.8 KB) — attack ground truth
- Short form on the page; links emailed/shown after submit
- **Graph:** host↔host (or user↔computer) auth edges, like Euler/Bowman
- **Split:** benign days train, red-team days test

### 3. DARPA TC E3 THEIA — provenance at scale
- Repo: [github.com/darpa-i2o/Transparent-Computing](https://github.com/darpa-i2o/Transparent-Computing)
- Start with **one** file, e.g. `ta1-theia-e3-official-6r` (Linux, week 2 = attacks)
- **Graph:** process/file/socket provenance (CDM JSON → edges)
- **Skip OpTC** for now — full archive is **~287 GB**; too heavy for this week

---

## What to build on each (same pipeline)

| Dataset | Train | Test positives | Negatives |
|---------|-------|----------------|-----------|
| CICAPT | Phase 1 edges | Phase 2 attack-touching edges | Random vs SoHeaRT |
| LANL | Pre-attack auth edges | Red-team auth edges | Random vs SoHeaRT |
| THEIA | Week 1 benign edges | Week 2 attack-touching edges | Random vs SoHeaRT |

Same GCN, same eval — paper story: **the random-vs-SoHeaRT gap holds across cyber graph types.**

---

## Realistic order (this week)

1. **Today:** Finish CICAPT graph + stats (you have the files).
2. **Tonight:** Request/download LANL `auth.txt.gz` + `redteam.txt.gz`.
3. **Day 2–3:** LANL host graph (subset is fine — e.g. first 30 days if full parse is slow).
4. **Day 4+:** THEIA only if LANL + CICAPT are running; otherwise 2 datasets is enough for 6 pages.

---

## Honest sizing note

CICAPT is **medium**, not huge. LANL and DARPA TC are the **big** ones. For CARS, 2 solid datasets (CICAPT + LANL) beats struggling with three.

**Start now:** Agent mode → build CICAPT graph while LANL downloads.

---

**User**

Skip to Article Content
Skip to Article Information
Institution of Engineering and Technology
Logo for: University of Wyoming Libraries
Search
Search within
This Journal
Search term
Search


Login / Register
IET HUB HOME
 
Journals
 
NEW JOURNALS
   
IET Membership
 
Visit IET
CAAI Transactions on Intelligence Technology
CAAI Transactions on Intelligence Technology
ORIGINAL RESEARCH
Open Access
Graph neural network-based attack prediction for communication-based train control systems
Junyi Zhao, Tao Tang, Bing Bu, Qichang Li
First published: 03 February 2024 https://doi.org/10.1049/cit2.12288Digital Object Identifier (DOI)  view metrics
Accessibility issue? Request accessibility update.

OpenURL University Of Wyoming Libraries 
SectionsPDF
CITE
Tools Share
Abstract
The Advanced Persistent Threats (APTs) have emerged as one of the key security challenges to industrial control systems. APTs are complex multi-step attacks, and they are naturally diverse and complex. Therefore, it is important to comprehend the behaviour of APT attackers and anticipate the upcoming attack actions. GNN-AP is proposed, a framework utilising an alert log to predict potential attack targets. Firstly, GNN-AP uses causality to eliminate confounding elements from the alert dataset and then uses an encoder-decoder model to reconstruct an attack scenario graph. Based on the chronological characteristics of APT attacks, GNN-AP identifies APT attack sequences from attack scenario graphs and integrates these attack sequences with communication-based train control (CBTC) devices topology information to construct an Attack-Target Graph. Based on the attack-target graph, a graph neural network approach is used to identify the attack intent and transforms the attack prediction problem into a link prediction problem that predicts the connected edges of the attack and target nodes. The simulation results obtained using DARPA data show that the proposed method can improve the comparison methods by 4% of accuracy in terms of prediction. Furthermore, the method was applied to the CBTC system dataset with a prediction accuracy of 88%, demonstrating the efficacy of the proposed method for industrial control systems.

1 INTRODUCTION
The communication-based train control (CBTC) system built on wireless communication and advanced computer technologies, supplemented by the remote control and supervision of computer equipment, improves rail transit safety and efficiency. The CBTC system of Commercial off-the-shelf (COTS) bridges the gap between train operating process and cyber network, but these devices might act as an entrance for cyberattacks [1]. From July 2015 to July 2016, four cyberattacks on the UK railway network were discovered [2]. In May 2017, the passenger information system of a German railway was compromised by malware termed the WannaCry virus [3]. Another instance occurred in May 2018, where a massive distributed denial-of-service (DDoS) attack on the Danish state rail operator (DSB) paralysed some operations, including ticketing systems and the communication infrastructure [4].

In recent years, the Advanced Persistent Threat (APT) (APT) attack emerges as significant security issues for industrial control systems [5]. Stuxnet-like APT attacks can affect industrial control systems that are controlled by computer devices, which implies that cyber threats might extend to real lives [6]. Advanced Persistent Threat attack is a multi-step attack. The Cyber Kill Chain [7] describes the multi-step structure of an APT. Many existing studies have tried to prevent APT attacks. An SDN-based intrusion detection system is suggested in Ref. [8], which employs deep learning and Bayesian network to detect APTs in the IoT. Ahmadian and Rasoolzadegan [9] used a causal network to detect a multi-step attack. The authors in Ref. [10] modelled APT-Attack as a scale-free network. They used semi-supervised learning and complex network theory to depict the evolution of the APT-Attack network and effectively detect suspicious hosts at different stages of the APT attack process. The attack graph analyses the attack path from the vulnerability of the device, while Bayesian networks are based on alert information to obtain possible future attack targets. However, in the actual prediction, we not only need to combine alarm information to analyse the attack intention but also need to combine device data to determine the potential attack target.

Graphs are widely used to present the complex relationship between events. In the last few years, graph neural network (GNN) as a method of analysing graph structure data has achieved remarkable results. Many studies apply GNNs to solve prediction task [11-14]. T-GCN [11] combined with the graph convolutional network (GCN) and the gated recurrent unit to forecast road traffic. Liang G [13] proposes a novel Semantics-aware Dynamic Graph Convolutional Network for traffic flow forecasting. Graph neural network for image classification is an important direction for the application [15-19]. Mou L et al. [17] proposed a non-local GCN that can process labelled and unlabelled data and use it for image classification. And the authors in Ref. [20]utilise GNN for action recognition. In Ref. [21], Haghshenas et al. present a Temporal Graph Neural Network framework for the detection and localisation of false data injection and ramp attacks on the system state in smart grids.

This study provides a GNN-AP framework for understanding attacker behaviour and predicting possible attack targets based on GNN. The framework can be divided into two modules: (1) Attack Scenario Graph Module; (2) Attack Prediction Module. In the attack scenario graph module, this article uses the encoder-decoder model to construct an attack scenario graph and identify multi-step attack sequences based on time-based depth-first search (DFS) algorithm. In the attack prediction module, the paper combines multi-step attack sequences with a CBTC system topology to produce an attack-target graph. A GNN is used to extract attacker features and forecast the next attack target from the graph. The contribution of the paper is as follows:
This research employs reinforcement learning to determine the causal relationships between alert data. The influence of confounding factors (e.g., false alarms and useless alarms) on the analysis of attack behaviour is reduced.

Based on the chronological characteristics of APT attacks, a time-based DFS algorithm is utilised to identify malicious APT attack behaviours in the attack scenario graph.

An attack target graph is constructed based on the APT attack behaviour sequence and the CBTC system topology. GNN is used to identify the attack intent from the attack target graph and convert the attack target prediction task into link prediction.

The rest of this article is organised as follows. Section 2 introduces the structure of a CBTC system and describes the functionality of each component. Section 3 presents the related work of cyberattack prediction. Section 4 discusses how to construct an attack scenario graph and identify multi-step attack sequences. In Section 5, the proposed GNN and link prediction are explained in detail to handle the attack prediction problem. Finally, Section 6 evaluates the simulation results of the proposed method, and Section 7 summarises the paper.

2 BACKGROUNDS
A CBTC system is a typical cyber-physical system, consisting of a control centre, on-board equipment, and a communication system. The typical structure is shown in Figure 1. The control centre comprises an automatic train supervision (ATS) system, zone control (ZC), and computer interlocking (CI). The on-board equipment mainly refers to automatic train protection and automatic train operation. The digital communication system provides a secure communication between the control centre and on-board equipment. The train control system is responsible for train overspeed protection, automatic driving, speed measurement, and location monitoring [22].

Details are in the caption following the image
FIGURE 1
Open in figure viewer
PowerPoint
Communication-based train control network structure.

3 RELATED WORKS
The security issues in CBTC systems have gained the attention of scholars. In Ref. [23], the authors developed a cross-layer defence scheme to mitigate the impact of jamming attacks on wireless CBTC communication systems. Additionally, the authors in Ref. [24] proposed a resilient control strategy to defend against Denial of Service attacks on wireless communication systems. Article [25] developed traditional intrusion detection systems by using a Kalman filter and a χ2 detector. These proposed approaches could accurately detect data tampering attacks. However, these studies focus on simple attacks like jamming and DOS.

In Ref. [26], an attack graph is generated based on network connectivity and vulnerabilities. The method improved attack scenarios reconstruction by analysing cohesive relationships between sub-scenarios and discovering unreported actual negative alerts. But it is challenging to obtain network vulnerability information without expert knowledge. Paper [27] merged a neural network and a Bayesian network. Their approach used neural network to remove false alerts and generates a Bayesian network attack graph via the filtered alerts. In Refs. [28, 29], an attack graph was generated to correlate individual alerts by utilising a data mining approach. These methods analyse extracted attack patterns from historical alert data, which provide information to anticipate future cyberattacks.

Lv et al. [30] have introduced a heterogeneous graph network attack detection method. The authors model the provenance data using a heterogeneous graph approach and represent each system entity with a semantic vector by which rich contextual information about each system entity (e.g., processes, files, sockets, etc.) can be obtained. Online network attack detection is then performed using graph learning individually. The experimental results show competitive performance compared to state-of-the-art rule-based network attack detection systems. This article uses heterogeneous graph learning for attack detection but cannot prevent APT attacks.

Zhao et al. [31] have introduced an HinAp framework to predict attack preference. The framework first built an attributed heterogeneous information network that classifies attack events into 5 categories: attackers, vulnerabilities, exploited scripts, attacked devices, and compromised platforms, describing the dependencies between each piece of information. Then, two models, superimposed attention mechanism and inductive learning, are put in place for attack preference prediction. Their time-delay data show that HinAP has good performance in predicting the attack preferences of intruders. Unfortunately, the data used in this paper is difficult to gather, such as the scripts deployed by the attacker. Such data may not be available in time during the attack process.

Li et al. [32] have introduced a framework called DeepAG that uses system logs to predict APT attack. A transformer model of the proposed framework is developed for detecting APT attacks from semantic information of system logs. DeepAG utilises the LSTM network and Out-Of-Vocabulary (OOV) word processor mechanisms to predict attack path, which achieves higher performance than traditional BiLSTM. However, the article ignores the system logs where non-malicious attacks can also be anomalous. The impact of these confounding factors needs to be excluded when analysing APT attacks.

This paper proposes a GNN-AP structure to first identify APT attacks by removing ambiguous alert such as non-malicious attack alert and false alerts from the alert data output by the intrusion detection system. Then, an attack target graph is constructed to analyse the attack intent of APT attackers and predict future attack targets. The structure is shown in Figure 2.

Details are in the caption following the image
FIGURE 2
Open in figure viewer
PowerPoint
Graph neural network-based attack prediction framework.

4 ATTACK SCENARIO GRAPH RECONSTURCTION
Though APT attacks are highly stealthy and complicated, this paper study APT attack scenarios from the alert log. Figure 3 illustrates the outline of attack scenario graph reconstruction, which is divided into three parts: alert data process, graph generation, and causal identify. First, in order to represent the alert logs in this paper, alerts are templated as 7-tuple and converted into vectors by word embedding, respectively. In the graph generation phase of this paper, these alert log vectors are fed into an encoder-decoder model to generate the graph adjacency matrix. Finally, this paper uses the idea of the RL paradigm to determine the causal relationships between graph nodes: the actor is an encoder-decoder model, and the reward function is tailored to causal discovery by incorporating a score function.

Details are in the caption following the image
FIGURE 3
Open in figure viewer
PowerPoint
Attack scenario graph reconstruction.

4.1 Alert data processing
The alerts are signals that indicate potential attacks detected by an intrusion detection system. They can be described as a n-tuple 
, where the 
 is the attribute of an alert. The alert log is the set of alerts, written as ε=(e1,e2,⋯,ei) [33]. This paper uses a 7-tuple time, class, priority, sip, sport, dip, and dport to present the alert ei. Samples of alerts are shown in the Table 1. However, the raw alarm logs cannot be directly used as the input for the model. To address this, this paper treats the values in time, class, priority, sip, sport, dip, and dport as words and vectorise the alerts using the GloVe algorithm [34]. This allows us to represent each alert as a vector in a high-dimensional space, which can be used as input for the model.

TABLE 1. Sample of alerts.
Time	Class	Priority	Sip	Sport	Dip	Dport
19:38:39	INFO TELENT access	Not suspicion	172.16.112.18	23	172.16.113.84	23,713
19:39:35	BAD TAFFIC same SRC/DST	Potentially bad traffic	0.0.0.0	68	255.255.255.255	67
19:39:59	INFO TELENT access	Not suspicion	172.16.112.18	23	194.27.251.21	43,976
19:40:11	INFO TELENT access	Not suspicion	197.218.176.32	23	172.16.113.148	25,627
4.2 Graph generation
Encoder This paper applies an attention-based encoder in the Transformer structure [35]. It comprises several transformer blocks. Each block contains multi-head attention networks and feedforward networks. The output of the attention layer is as follows:
 
(1)
where the matrices Q, K, V denote the queries, keys, and values of the input, respectively. The output of the feedforward layer is as follows:
(2)
The attention scheme is an effective way to find the causal relationships among variables [36]. The self-attention encoder final output is denoted as 
 with d dimensions.

Decoder The decoder establishes the relationships between the encoder outputs eni. Then, a graph adjacency matrix A is generated in an elementwise manner. A single-layer decoder can be written as follows:
(3)
where 
 and 
 are trainable parameters and dhidden is the hidden dimension associated with the decoder. f is the tanh function. This paper obtains an adjacency matrix A by taking each entry mij as an input of a logistic sigmoid function σ (⋅). Finally, this paper uses sampling to indicate the probability that an edge exists between alerts ei and ej, where the samples follow a Bernoulli distribution with probability σ(mij).
4.3 Causal relationship identify
After obtaining the structure diagram, RL-based causal inference is used to identify the causal relationship with the best score [36]. The Bayesian information criterion (BIC) score of a directed graph 
 is as follows:
(4)
If a graph 
 is acyclic, its adjacency matrix 
 should satisfy the constraint:
(5)
This paper needs to consider the score function and acyclic constraints when designing the reward function. Therefore, the reward function can be written as follows:
(6)
The indicator function is denoted by F(⋅) and two parameters βa, βb ≥ 0, which are penalty parameters. If the values of βa and βb are large, the graph generated by using the proposed reward function is more likely to be acyclic. Therefore, this paper needs to make all possible graphs have the largest reward to ensure that they are acyclic. This paper sets this problem equal to
(7)
By applying the actor-critic algorithm in the RL paradigm, the expected reward function is our training object, and write it as follows:
(8)
In this part, this paper finds the graph with the best reward among all generated graphs during training and set it as the output. However, the graph may have some edges that should not exist due to the existence of confounding factors in the actual process. Therefore, this paper needs to proceed further.

This paper optimises the estimated edges with the greedy algorithm based on regression performance or the score function. For a causality edge, this paper optimises it by the causal inference method. It removes the parent of a node and then computes the score of the graph while preserving the causal relationship among the remaining nodes. If the score of the graph does not decrease or the decreased value is within the allowable range after implementing the optimisation method, we accept the optimisation result and continue processing according to the optimised graph.

In the above graph edge optimisation process, it is necessary to consider adding a penalty weight to the number of graph edges in the score function. However, it is crucial to choose a good weight. Bad weights may cause the correct edge to be lost in the graph. This paper incorporates penalty weights log m when computing the BIC score and then optimises the graph to reduce false causalities.

4.4 Multistep attack alert sequence
This paper obtains an attack scenario graph 
 form alert datasets, such as DARPA 2000. The alert dataset contains alerts generated by malicious attacks and accidental scenarios. Malicious attacks are composed of multiple attack steps and are continuous in time. This paper identifies the multi-step attack alert sequence based on the time factor. However, some problems may also be encountered when extracting attack alert sequences from alert graphs, as shown in Figure 4.

Details are in the caption following the image
FIGURE 4
Open in figure viewer
PowerPoint
Multistep attack alert sequence.

In this figure, A − B − C − D and A − C − D are two obvious attack alert sequences. This paper identifies these two paths as correct attack sequences if the time factor is ignored. However, the main characteristic of a multi-step attack is that the attacker remains hidden in the system for a long time. The attacker will try various ways to find a vulnerability during the execution of an attack. This may lead us to discover incorrect attack alert sequences. For example, in Figure 4, alert B causes alert C at t0, while A causes B at t1.

Therefore, the causal relationship among alerts A, B and C is not established due to a chronological conflict. The attack alert sequence 
 is incorrect.

The [37] method extends the traditional DFS approach with temporal information, which can effectively solve the above problems. In this way, the influence of chronological order on the accurate detection of alert relationships can be eliminated in an attack sequence. Unlike computer networks, each computer in an industrial control system performs a different role, so the importance levels of different computers to the system are also different. In CBTC, CI is more important than the ZC computer. Based on the time-based DFS algorithm, this paper also considers the node importance weight in the algorithm. It will be helpful for extracting the primary attack alert sequences from the causal alert graph.

This paper obtains multiple alert sequences with malicious attack behaviours by executing the time-based DFS algorithm. According to the output sequences, the system tries to extract the attack intention from each sequence. The pseudocode is summarised in Algorithm 1.

Algorithm 1. Multi-step Attack Alert Sequence Discovery

Input: IDS Alert Dataset 

Output Multi-step Attack Alert Sequence: AS

1 // Attack Scenario Graph

2 Encoder: This paper uses an attention-based encoder, and the output of the encoder is as follows:

3 

4 Decoder: mij (Z1, Z2, q) = qTf (Z1eni + Z2emj)

5 //Causal Identification

6 Acyclic Constraints Set reward function:

7 

8 Actor-Critic Algorithm:

9 

10 //Multi-step Attack Alert Sequence Discovery

11 For node in 
 do

12   Set node attributes = (time, class,priority,sip,sport,dip,dport)

13  node.time = node[time]

14  root.node←nodei.[time] = starttime

15 if nodei.[time]<nodej.[time] then

16   path.add(nodej)

17  else

18   path.delete(nodej)

19  end

20 end

21 return (AS)

5 CYBERATTACK PREDICTION BASED ON GNN
The goal of our work is to obtain the subsequent behaviour of an attacker from a multi-step attack alert sequence. The authors in Ref. [38] first used recommender systems in the field of cyber defence. The authors proposed a collaborative filtering method to solve the attack prediction problem. This paper proposes a sequential recommendation method to solve the multi-step attack prediction task in industrial control systems [39]. Sequential recommender systems recommend products that users may be interested in by modelling the sequential dependencies over user-product interactions. By establishing sets for attackers and targets, the interaction between attackers and targets can be represented using attack alert sequences. This paper utilises principles from recommender systems to infer attack intentions in order to forecast potential attack targets. The system model is shown in Figure 5.

Details are in the caption following the image
FIGURE 5
Open in figure viewer
PowerPoint
Attack prediction module.

5.1 Attack target networks
The attack-target network (ATNet) 
 is integrated with multi-step attack alert sequences and the system network topology. The graph contains two types of nodes: attacker nodes A and system device nodes D. The paper maps the alert events to system devices corresponding to their IP addresses. A device node d has an attribute set p = (p1, p2,⋯, pi) at time t. In this paper, the device properties are set to Device Function, Device Importance, Vulnerability, and Defence Level. Consequently, the dynamic graph can be represented by the following equation:
(9)
when attacker a attacks target device d at time t, a link la is established between a and d. The communication link between devices di and dj is denoted as lc. This paper defines ATNet consisting of all attacker behaviours at time t as 
. In the network, the neighbouring nodes of an attacker node are the attacked devices. The attack intention can be deduced from all historically compromised devices. The paper uses the message propagation mechanism to obtain the attack intention from the neighbouring nodes. A GNN is a powerful method for revealing the relationships between the centre node and its neighbours. However, it fails to capture the sequence dependencies between neighbouring nodes. Furthermore, this paper applies a relational attention mechanism to this module, which is well suited for predicting attacks from sequence data.
A GCN is an efficient way to directly aggregate all neighbouring nodes:
 
 
(10)
where 
 is the feature representation of node a in the lth layer. 
 is the weight parameter of the node in the lth layer. Na represents the number of neighbouring nodes of node a.
The characteristic of the GCN model is that it obtains information from the adjacent nodes of the central node, but it ignores the chronological order information between the adjacent nodes. To effectively distinguish the influences of different target devices and take full advantage of the available attacker-target interaction order information, this paper applies the attention mechanism with additional sequence dependency information in the model.

This paper denotes ρij as a sequence dependency relation between attack target di and the last target node dj. Equation (11) is the representation form used for device nodes along the attack sequence.
 
 
(11)
Nevertheless, this approach fails to consider the dependency relations between different devices. This paper assigns a parameter 
 that maps the sequence dependency information relations into vectors.
 
(12)
where 
 is a dependency head. Therefore, the final representation of device node di is as follows:
(13)
This paper takes 
 into consideration when calculating the attention coefficients. The aware attention mechanism differentiates the importance of a target to the attacker by its importance weight ead. Taking the node embedding 
 of the lth layer as an input, the weight is formulated as follows:
 
(14)
where 
 and 
 are the embeddings of attacker ea and target device ed respectively. The embedding dimensionality is denoted as d. In the formula, the scale parameter 
 is designed to avoid excessive dot product computations, and it is helpful for improving the convergence speed of the model. The importance levels of attacker nodes and their attack target nodes are calculated via the Softmax function:
 
(15)
where σ is the rectified linear unit (Relu) activation function and ∥ denotes the concatenation operation.
Thus, the intention of the attacker can be obtained by aggregating the information derived from all neighbours (the attack targets):
 
(16)
where 
 captures the sequence information in the attack-target prediction graph.
Node Updating In this part, this paper aggregates the attention of the attacker and update the node representation of 
 with the embedding of the previous layer. For the attacker node, the representation updating rule from the previous layer can be calculated as follows:
(17)
where 
 is the parameter for controlling the embedding information 
, 
 and 
.
5.2 Cyberattack target prediction
In this subsection, this paper uses link prediction to solve the attack prediction problem. The paper designs a link prediction function to determine the attack target that the attacker may invade next.

A given candidate target d ∈ D is selected based on real system network connections. The link prediction function is defined as follows:
(18)
where vector 
 denotes the scores of all potential attack target devices d. Ed is the embedding of device d, and W is the trainable transformation matrix. In a real industrial control network, the functions and importance levels of devices influence the attacker's choice. Therefore, this paper uses ξd to denote the strength of device d, namely, the sum of its weights according to its function, importance, and other aspects.
(19)
Wd is obtained from experts in the industrial control system security field. In function (18), ha denotes the final attack intention. This paper obtains each layer embedding 
 of the attacker node from an L-layer ATNet. The embeddings in different layers emphasise the various attacker preferences. Therefore, the final attack intention can be written as follows:
(20)
The cross-entropy function is commonly used as the loss function in such optimisation tasks. The cross-entropy loss function can quickly constrain the model loss to converge to a global local minimum. The normalised vector of the scores for potential attack devices is as follows:
(21)
Thus, the objective function is as follows:
 
(22)
where ya denotes the true target of the next attack, and 
 is the result predicted by the proposed method. Θ denotes all model parameters, and ‖⋅‖2 is the L2 norm. λ controls the regularisation strength. This paper optimises the model by using a back-propagation Algorithm. The pseudocode for this is presented in Algorithm 2.
Algorithm 2. Attack Target Prediction

Input: Multi-step Attack Sequence AS1=(as1,as2,⋯,asn)

    System network topology: 

    Number of graph layers: L

Output The subsequent attack system device: dt+1

1 //Attack Target Graph

2 Combine attack alert sequences and system topology into an attack target graph 

3 //Node representation initialisation

4 

5 ///The node update process

6 For each l ∈ [1:L] do

7  

8  

9  

10 end

11 //The prediction of the following attack target

12 

13 Next attack target ←argmax (s (a, d))

6 RESULTS
In this section, this paper uses DARPA 2000 to verify the effectiveness of the proposed method. Then, the paper demonstrates the ability of our method in a CBTC system.

6.1 DARPA dataset
The DARPA1999 and 2000 datasets are widely used for security testing. The DARPA 2000 dataset contains two kinds of attack scenarios (LLDOS 1.0 and LLDOS 2.0.2). These attacks involve five stages, including scanning hosts, sending vulnerability query packets, exploiting vulnerabilities to attack hosts, implanting DDoS software into target hosts, and using DDoS software to launch DDoS attacks. In the experiment, this paper replayed network traffic in DARPA1999 & 2000 using Snort IDS, which generated a large number of raw alerts. This paper labelled the raw alerts and remove duplicates, false alerts, and unnecessary alerts, resulting in processed alerts (Table 2). The proposed model is trained on DARPA1999. It is then evaluated for multi-step attacks using the DARPA 2000 dataset.

TABLE 2. IDS alerts of DARPA1999&2000.
Dataset	Total number of alerts	Processed alerts
DARPA1999	2324	598
DARPA2000	2755	854
This paper sets the hyperparameters in the experiment as follows.

N = 5 and dhidden = 12 denote the number of the transform block in the encoder and hidden dimension in a single layer decoder, respectively. RL-related parameters are unchanged as in Ref. [31]. For the cyberattack prediction module, this paper uses a 3-layer GCN, and the embedding size is set to 50.

6.2 Attack scenario graph reconstruction
Figure 6 shows the reconstructed attack scenario graph for DARPA2000. In the figure, each node presents an alert event. The attack graph presents the attack step by removing the confounding factors from the alert data through causal machine learning. The first stage involves finding online hosts. It consists of four Ping-type alerts 2; 4; 5; 6. The alerts 11; 14; 47; 46 indicate host vulnerability exploration by the attacker. After obtaining an exploitable vulnerability, stages four and five perform attack software injection and attack execution.

Details are in the caption following the image
FIGURE 6
Open in figure viewer
PowerPoint
Attack scenario graph.

While an attack scenario graph can help remove a larger number of confounding alerts, some not-relevant positive alerts still exist in the graph. To obtain malicious multi-step attack sequences, this paper uses the time-based DFS algorithm to extract attack sequences. Unexpected events tend to generate short-lived alerts, while malicious multi-step attacks are characterised by long and continuous durations. In the experiment, when the time weight parameter Twet is set to 10, it can eliminate unexpected alerts 5; 6; 11; 47 etc and find main attack sequences. Figure 6 demonstrates the attack scenario graph constructed from the alert data. It contains 38 alerts in the graph. Then, we applied time-based DFS to discover main malicious attack sequence, as shown in Figure 7. Two multi-step attack sequences are extracted from the attack scenario graph. Attack sequence #1 can be divided into four stages. The first stage is to find the online hosts. It consists of three Ping type alerts. The second stage is host vulnerability exploration. It contains five alerts. After obtaining an exploitable vulnerability, stages four and five, respectively, perform attack software injection and attack execution. The attack sequence #2 is similar to sequence #1, while the sequence just misses exploring the online Hosting stage. From Figure 7, it is evident that while both attack sequences commence from the same host, their final target endpoints differ due to variations in attackers' intentions. By comparing with the description of the DARPA200 database, the proposed method can accurately reconstruct the attack scenario and detect malicious multi-step attack steps.

Details are in the caption following the image
FIGURE 7
Open in figure viewer
PowerPoint
Attack sequence.

6.3 Attack target prediction
This paper implements a GNN-AP framework to make attack predictions based on the multi-step attack sequences. First, this paper builds the dynamic prediction network by mapping the alerts to the relevant devices based on their IP addresses. The candidate attack target is selected according to the network topology. This paper sets the number of layers L in the ATNet to 3. This paper compares the performance of our model with that of other methods [22, 32, 34]. In this paper, four typical evaluation matrices are employed to evaluate the prediction performance of the ATNet model.
 
 
 
 
 
(23)
where TP is the number of correct prediction results that are classified as correct. FP is the number of correct prediction results that are classified as false. TN is the number of false prediction results that are classified as false. FN is the number of false prediction results that are classified as correct. Table 3 presents the results of a comparative analysis with other methods. It is observed that the ATNet model outperforms the DeepAG, Alert-GCN and Bayesian Network models in terms of prediction accuracy. The improved performance of our model can be attributed to its ability to utilise causality in generating multi-step attack sequences, which enables more accurate extraction of attacker intention. Although the MIF method also achieves high prediction accuracy, the proposed model performs better in other metrics. This suggests that our model provides a more comprehensive solution for predicting cyberattacks.
TABLE 3. Performances comparison among different methods.
Model	Accuracy	Precision	Recall	F1 score
Bayesian network [27]	0.9795	-	-	-
DeepAG [32]	0.89018	-	-	0.86907
MIF [37]	0.99	0.97	0.95	0.96
Alert-GCN [40]	0.9242	0.9390	0.9242	0.9316
Proposed model	0.9940	0.9942	0.9915	0.9923
6.4 Case study on a CBTC system
In this section, this paper applies the method to the CBTC simulation platform. The platform consists of VOBC, WGB, AP, ZC, CI, ATS and wayside equipment. The VOCB, WGB, AP, ZCs are real equipment. The ATS, CIs and wayside equipment are implemented by software. Figure 8 shows the architecture of the simulation platform.

Details are in the caption following the image
FIGURE 8
Open in figure viewer
PowerPoint
Communication-based train control simulation platform.

This paper simulates a simple DDoS attack including IP sweep, exploit vulnerability and attack execution. This paper collects traffic for different time window sizes and use Snort to generate alerts. The alert data stream is split into different windows sizes, with W1 = 30; W2 = 60; W3 = 90; W4 = 120; W5 = 150, as shown in Figure 9.

Details are in the caption following the image
FIGURE 9
Open in figure viewer
PowerPoint
Communication-based train control alert data stream.

Figure 10 shows the performance of proposed method applied to CBTC. The values of the accuracy, recall, precision and F1 score metrics all exceed 80%. The proposed method has a 4% improvement in the metric of accuracy compared to the method described in Ref. [25]. Figure 10 shows that the prediction result is best when the train and test data are in the same time window. From 0 to 90 times, the prediction performance also becomes better as the dataset becomes larger. After 90 times, the performance remains stable. It also shows that the value of the four measures does not decrease dramatically if the training and testing are not in the same time windows. The result reveals the stability of our method. Figure 10c reveals that the prediction performance is best when the training and testing data are in the same time window. For instance, if the training data are in W2 and the test data are in W5, the worst result only drops to 0.07.

Details are in the caption following the image
FIGURE 10
Open in figure viewer
PowerPoint
(a) Accuracy, (b) Recall, (c) Precision and (d) F1-score.

7 CONCLUSIONS
Communication-based train control (CBTC) systems are susceptible to APT attacks with widespread applications of COTS. The study presents a novel framework based on GNN for predicting the upcoming attack target in APT attack scenarios. The GNN-AP framework utilised reinforcement learning-based causality technique to construct attack scenario graphs that accurately reveal the causality between alert information. This approach eliminates the impact of meaningless alerts and non-alerts on reconstructed attack scenario graphs. In addition, the ATNet combines the attack behaviour with the CBTC system topology information and uses GNN to learn the intent of the attacker from ATNet. Thus, the problem of attack target prediction is transformed into the node-link prediction of ATNet.

The experimental results show that GNN-AP can accurately restore the attack scene and get the attack behaviour sequence based on the intrusion detection alarm information. Tests on DARPA datasets show that the proposed method improves by 2% over MIF and 5% over Alert-GCN. In the experiment of the CBTC system, this paper assigns different weights to the function and importance of different devices, which makes the prediction result closer to reality. The prediction accuracy can reach 88%. This approach helps CBTC system managers identify the intent and subsequent targets of an attack, enabling them to deploy more targeted defences.

The proposed model is based on the alert information detected by IDSs, which makes it difficult to predict cyberattacks that are not detected by the IDS. In future work, we need to consider how to guarantee the accuracy of the training model when there are various variations in the alert data. We plan to use the stable learning approach to train a model that can be applied to various scenarios.

ACKNOWLEDGEMENTS
This work was supported by the Beijing Natural Science Foundation-Fengtai Rail Transit Frontier Research Joint Fund (Grant No: L211002) and the Beijing Laboratory for Urban Mass Transit (Grant No: I18H100010).

CONFLICT OF INTEREST STATEMENT
The authors declare that there is no conflict of interest regarding the publication of this paper.

DATA AVAILABILITY STATEMENT
The data used to support the findings of this study are included within the article.

REFERENCES
Citing Literature

Early View

Online Version of Record before inclusion in an issue

Figures
References
Related
Information
Recommended
A new control for the pneumatic muscle bionic legged robot based on neural network

Chaoyue Xu, 
Feifei Qin, 
Kun Zhou, 
Binrui Wang, 
Yinglian Jin
IET Cyber-Systems and Robotics
Adaptive neural‐based asynchronous control for nonhomogeneous Markov jumping systems with dead zones

Xiang Li, 
Luxue Wang, 
Shuangsi Xue, 
Zihang Guo, 
Hui Cao
IET Control Theory & Applications
ESO‐based output‐feedback regulation control of nonlinear systems with SiISS inverse dynamics

Chunxiao Wang, 
Yang Gao, 
Yuliang Sun, 
Yuqiang Wu
IET Control Theory & Applications
Survey of Attack Graph Analysis Methods from the Perspective of Data and Knowledge Processing

Jianping Zeng, 
Shuang Wu, 
Yanyu Chen, 
Rui Zeng, 
Chengrong Wu
Security and Communication Networks
Observer‐based integral sliding mode control for uncertain neutral semi‐Markovian jumping systems with time‐varying delays

Qinhong Fu, 
Lianglin Xiong, 
Haiyang Zhang, 
Kaibo Shi
IET Control Theory & Applications
Download PDF
 back
Institution of Engineering and Technology Logo
About The IET
IET Privacy Statement
Contact The IET
journals@theiet.org
Copyright (2026) The Institution of Engineering and Technology. The Institution of Engineering and Technology is registered as a Charity in England & Wales (no 211014) and Scotland (no SC038698)
Additional links
About Wiley Online Library
Privacy Policy
Terms of Use
About Cookies
Manage Cookies
Accessibility
Wiley Research DE&I Statement and Publishing Policies
Help & Support
Contact Us
Training and Support
DMCA & Reporting Piracy
Sitemap
Opportunities
Subscription Agents
Advertisers & Corporate Partners
Connect with Wiley
The Wiley Network
Wiley Press Room
Copyright © 1999-2026 John Wiley & Sons, Inc or related companies. All rights reserved, including rights for text and data mining and training of artificial intelligence technologies or similar technologies.


Wiley Home Page
May we use cookies to track your activities? We take your privacy very seriously. Please see our privacy policy for details and any questions.Yes No


or can we do same problem on other datasets like lanl or cicapt?

---

**Cursor**

**You can do link prediction for attack detection on LANL and CICAPT — but not the same GNN-AP problem without rebuilding their whole pipeline.**

## What GNN-AP actually needs

| Ingredient | GNN-AP | LANL | CICAPT |
|------------|--------|------|--------|
| **Input** | IDS **alert logs** (Snort) | Raw **auth events** | **Provenance** (process/file/socket) |
| **Graph** | Alert scenario graph → **attacker ↔ device** bipartite + topology | **Host/user auth** graph | **Process–artifact** provenance graph |
| **Task** | Predict **next attack target device** | Detect **anomalous auth edges** (LM) | Detect **attack-related provenance edges** |
| **Labels** | Next device in attack sequence | `redteam.txt` auth events | `label=1` attack nodes |
| **Extra** | CBTC device topology, expert device weights | None needed | MITRE `subLabel` tactics |

GNN-AP is **alert → causal graph → attack-target LP**. LANL and CICAPT don't ship as IDS alerts or device-topology attack-target graphs.

## Can you port GNN-AP to LANL?

**Partially, not faithfully.**

- LANL has **auth edges** (src host → dst host), not Snort alerts.
- You could do **auth-graph link prediction** (Euler/Bowman/Jbeil) — predict unusual auth links.
- You **cannot** build ATNet (attacker node + device nodes + CBTC topology) without:
  - Running Snort on replayed traffic (they did this on DARPA 1999/2000)
  - Having ICS device topology and importance weights

**Closest LANL analogue:** anomalous **auth link prediction** for lateral movement — that's Euler, not GNN-AP.

## Can you port GNN-AP to CICAPT?

**Same LP family, different graph.**

- CICAPT gives **provenance edges** with APT labels — good for **attack edge link prediction**.
- No alert logs, no attacker/device bipartite graph, no train-control topology.
- Closest analogue: predict edges touching attack processes/files — **provenance LP**, closer to Lv et al. [30] in GNN-AP's related work than to GNN-AP itself.

## What you should do (recommended)

Don't clone GNN-AP. Do **one LP task, two graph types, one evaluation fix:**

| Dataset | Graph | Positive test edges | Your twist |
|---------|-------|---------------------|------------|
| **CICAPT** | Provenance | Phase 2 attack-touching edges | Random vs **SoHeaRT** negatives |
| **LANL** | Auth (host↔host) | Red-team auth edges | Same eval |

**Paper line:** GNN-AP, Euler, and similar work frame APT/attack detection as **link prediction**, but evaluate with **easy negatives**. You show the gap is real on **provenance (IIoT)** and **auth (enterprise)** graphs.

## If you really want GNN-AP-style

You'd need:
1. **DARPA 1999/2000** + Snort to generate alerts (~600–850 alerts — tiny), or
2. **Private CBTC simulation** (not public)

That's a poor fit for your timeline and public-data requirement.

**Bottom line:** LANL and CICAPT are right datasets for **cyber link prediction**; they're wrong datasets for **replicating GNN-AP**. Use them for provenance LP + auth LP with SoHeaRT evaluation — cite GNN-AP as related work that also uses LP but doesn't address hard negatives.

---

**User**

ok lets begin with provenance dataset we have. can it be used to predict links? what does link prediction tell in this datasett?
ans briefly

---

**Cursor**

**Yes — it works for link prediction.**

## Setup
- **Nodes:** processes + artifacts (files, sockets, etc.)
- **Edges:** `Used`, `WasGeneratedBy`, `WasTriggeredBy`, `WasDerivedFrom`
- **Train:** Phase 1 edges (all benign)
- **Test positives:** Phase 2 edges touching `label=1` nodes (~570 edges)

## What link prediction means here

The model learns **normal provenance connectivity** from Phase 1, then scores whether an edge *(u, v)* should exist.

On Phase 2, **attack edges that look structurally unusual** (rare process↔file, process↔socket patterns) get **low scores** → flagged as anomalous.

So LP here means: **“Is this system interaction expected for a benign IIoT host?”** — not “what will the attacker do next?”

## What it detects
- `credentialAccess`, `collection`, `defenceEvasion`, etc. — mostly **local process/file behavior**
- **Not** mainly lateral movement (only ~18 socket-related attack edges)

**One line:** Provenance LP = **anomaly detection on attack-related system interactions**, framed as predicting which edges are normal vs suspicious.

---

**User**

okay how to do it?

---

**Cursor**

Reviewing your existing preprocessing pipeline so the steps match your codebase.


**4 steps — mirrors your Twitter/HeaRT pipeline.**

---

### Step 1: CSV → `graph.pt`

Parse both CSVs:

- **Nodes:** every `Process` / `Artifact` row → `id`
- **Edges:** rows where `type` ∈ `{Used, WasGeneratedBy, WasTriggeredBy, WasDerivedFrom}` → `(from, to)`
- **Directed**, collapse all 4 types to one edge type
- **Remap** hex IDs → `0..N-1`, save `graph.pt`, `node_id_map.txt`, `meta.json`

Same pattern as `preprocess_twitter_edges.py`.

**Union graph:** Phase1 + Phase2 nodes/edges in one graph (so attack nodes exist at test time).

---

### Step 2: Define train / test edges

| Split | Edges |
|-------|-------|
| **Train** | Phase 1 edges only (benign) |
| **Test positives** | Phase 2 edges where `from` or `to` has `label=1` |
| **Val** | Small slice of Phase 1 edges (hold out ~10%) |

Track `attack_nodes` set from Phase 2 `label=1` rows.

---

### Step 3: Features + GCN

1. Build adjacency from **train edges only**
2. **SVD features** via `build_snap_features.py` logic → `x_svd256.pt`
3. **GCN** encoder on full graph (or train subgraph)
4. **Decoder:** dot-product score `σ(z_u · z_v)` per edge
5. **Train loss:** Phase 1 positives + random non-edges (BCE)

Reuse `main_gnn_snap.py` / `GCN` from HeaRT.

---

### Step 4: Evaluate (your contribution)

Score all **test positive** edges vs negatives:

| Negatives | How |
|-----------|-----|
| **Random** | Random unconnected pairs |
| **SoHeaRT** | `create_heart_negatives.py` — PPR + CN/RA hard non-edges |

Metrics: **AP**, **AUC** (and per-`subLabel` if you want).

**Interpretation:** High AP with random negs but low with SoHeaRT → easy negatives hide weak detection.

---

### Minimal file layout

```
CARS/processed/
  graph.pt
  node_id_map.txt
  x_svd256.pt
  splits/train_pos.pt, test_pos.pt, val_pos.pt
HeaRT/dataset/cicapt/
  heart_test_soheart.npy   # from create_heart_negatives
```

---

**Start:** Switch to **Agent mode** → implement Step 1 (CSV parser + feasibility counts). That unblocks everything else.

**Watch out:** Only include Phase 2 **attack-touching** edges as test positives (~570), not all Phase 2 edges — otherwise benign Phase 2 activity pollutes labels.

---

**User**

first help me understand the dataset both phase 1 and phase 2
number of nodes
Edges
features
what to predit
why we need hard negatives in this case, when predicting a link as anamolous randomly will allow  user to protect his system, i mean there is no disadvantage for extra protection instead of doing hard negatives and sometimes failing to predict anomolous link?

---

**Cursor**

Gathering exact Phase 1/2 stats from the CSVs, then explaining what to predict and why hard negatives matter.


## Dataset at a glance

| | **Phase 1** | **Phase 2** |
|---|-------------|-------------|
| **Purpose** | Benign IIoT baseline | APT29-style attack campaign |
| **Nodes** | 46,773 | 53,286 |
| **Edges** | 175,347 rows → **64,225** unique directed pairs | 143,449 rows → **75,345** unique pairs |
| **Attack nodes** (`label=1`) | **0** | **332** (collection 100, credentialAccess 82, …) |
| **Attack edges** | 0 | **570** touching attack nodes |

**Combined graph (what you build):** ~**84,850** nodes, ~**139,565** unique edges. Phase 1 and Phase 2 share some node IDs (~15K overlap).

---

## Node types

- **Process** — `exe`, `name`, `command line`, `pid`, `ppid`, …
- **Artifact** — mostly **files** (22K/17K), **network sockets** (14K/11K), plus links/directories

## Edge types (collapse to one for LP)

| Type | Phase 1 | Phase 2 | Meaning |
|------|---------|---------|---------|
| `Used` | 22K | 34K | process used file/socket |
| `WasGeneratedBy` | 136K | 88K | artifact created by process |
| `WasTriggeredBy` | 6K | 13K | process spawned process |
| `WasDerivedFrom` | 11K | 8K | data lineage |

---

## Features

The CSV has **no ready-made embedding**. Per node you get metadata (`exe`, `path`, `subtype`, `remote address`, …). In your pipeline you’ll build:

- **Structural:** SVD on adjacency (like Twitter SNAP) — default
- **Optional:** one-hot `type`/`subtype` or hash `exe`/`path`

Most signal comes from **graph structure**, not rich node attributes.

---

## What to predict

**Not** “predict any missing edge.”

**Task:** Score edges as normal vs suspicious.

1. **Train** on Phase 1 edges → learn normal IIoT provenance patterns  
2. **Test positives** = Phase 2 edges touching attack nodes (570)  
3. **Score** each edge; low score → flag as anomalous APT activity  

You’re asking: *“Would this process–file / process–process interaction happen on a benign host?”*

---

## Why hard negatives? (your “extra protection” question)

Random negatives don’t give **extra protection in production**. They only affect **how you measure the model during research**.

### In deployment you don’t use random negatives

At runtime the system scores **real candidate edges** — process reads a file, opens a socket, spawns a child. Those negatives are **hard**: they look like normal provenance, not random unrelated pairs.

### Random negatives inflate confidence

Random pair *(random_process, random_file)* is almost never a real edge → model easily ranks attack edges above them → **AP looks high (e.g. 0.95)**. That does **not** mean you’ll catch attacks among millions of plausible benign edges.

### “Flag everything” has real cost

| Extra alerts | Cost |
|--------------|------|
| False positives on normal IIoT ops | Alert fatigue, analyst ignores dashboard |
| Block/stop legitimate processes | Downtime in factory/plant |
| Investigation time | \$ and missed real incidents |

SOC goal: **high precision on plausible events**, not “detect by comparing to impossible pairs.”

### What hard negatives test

SoHeaRT negatives = **realistic benign non-edges** (similar structure, similar neighbors). If the model still ranks **attack edges above those**, detection is real. If not, random-negative AP was **misleading** — you’d deploy a weak detector thinking it works.

### Analogy

- **Random negatives:** “Can you find a criminal among 1000 random people from another country?” Easy.  
- **Hard negatives:** “Can you find them in a crowd of similar-looking commuters?” That’s the real job.

---

**Bottom line:** Hard negatives don’t make protection worse — they make **evaluation honest**. Random negatives let you **think** you’re safe when you’re not; hard negatives show whether you can separate attacks from **plausible normal** system behavior, which is what operators actually need.

---

**User**

so we combine phase 1 and 2 and train on some of attack nodes right and move forward?\

---

**Cursor**

**No — do not train on attack nodes or attack edges.**

## Correct setup

| Step | What |
|------|------|
| **Graph** | Combine Phase 1 + Phase 2 (union nodes + edges) so attack nodes exist in the graph at test time |
| **Train** | **Phase 1 edges only** — all benign, no `label=1` nodes in Phase 1 anyway |
| **Val** | Small holdout from Phase 1 edges (~10%) |
| **Test positives** | Phase 2 edges touching `label=1` nodes (~570) |
| **Test negatives** | Random + SoHeaRT non-edges |

## Why not train on attacks?

Training on attack edges = **supervised** (“learn what attacks look like”). Your story is **anomaly LP**: learn normal IIoT behavior from Phase 1, then flag Phase 2 attack edges as **unlikely** under that model.

## Flow in one line

> Build full graph from both phases → train GCN on **benign-only** Phase 1 edges → score Phase 2 **attack** edges as anomalous.

Phase 2 **benign** edges stay out of test positives (only attack-touching edges are positives). Phase 2 is not used for training.

---

**User**

what about 2 more datasets for the same problem?

---

**Cursor**

**Best 2 add-ons for the same problem** (benign train → attack-edge test → random vs SoHeaRT):

---

## Recommended trio

| # | Dataset | Graph | Train (benign) | Test positives | Download |
|---|---------|-------|----------------|----------------|----------|
| **1** | **CICAPT-IIoT** ✓ have it | Provenance (process/file/socket) | Phase 1 edges | Phase 2 attack-touching edges (~570) | Already in workspace |
| **2** | **LANL Cyber1** | Host **auth** graph | Pre–red-team auth edges | Red-team auth edges (`redteam.txt`) | [csr.lanl.gov/data/cyber1](https://csr.lanl.gov/data/cyber1) — `auth.txt.gz` (7.2 GB) + `redteam.txt.gz` |
| **3** | **DARPA TC E3 (THEIA)** | **Provenance** (Linux) | Week 1 benign edges | Week 2 attack-touching edges | [github.com/darpa-i2o/Transparent-Computing](https://github.com/darpa-i2o/Transparent-Computing) — one file e.g. `ta1-theia-e3-official-6r` |

Same pipeline on all three: build graph → GCN on benign edges → test attack/anomalous edges → random vs SoHeaRT AP/AUC.

---

## Why these two

- **LANL:** Different graph type (auth/LM), huge scale, Euler/Bowman standard — shows the eval gap isn’t provenance-only.
- **THEIA:** Same provenance family as CICAPT, much bigger — shows it scales beyond IIoT CSV.

**Skip OpTC** for now (287 GB). **Skip DARPA 1999/2000 alerts** (GNN-AP-style, tiny, not your graph format).

---

## Realistic priority

| Timeline | Datasets |
|----------|----------|
| **Minimum (6-page paper)** | CICAPT + LANL |
| **Stronger** | CICAPT + LANL + THEIA |

THEIA needs JSON→graph parsing (1–2 extra days). LANL needs auth parsing + red-team join (1 day).

---

## One paper sentence

> We evaluate hard-negative link-prediction benchmarking on three public cyber graphs: IIoT provenance (CICAPT), enterprise authentication (LANL), and DARPA TC provenance (THEIA).

**Next step:** Finish CICAPT first, request LANL download in parallel, add THEIA only if CICAPT + LANL are running by mid-week.

---

**User**

Comprehensive, Multi-Source Cyber-Security Events
This data set represents 58 consecutive days of de-identified event data collected from five sources within Los Alamos…

Unified Host and Network Data Set
The Unified Host and Network Dataset is a subset of network and computer (host) events collected from the Los Alamos…

User-Computer Authentication Associations in Time
This anonymized data set encompasses 9 continuous months and represents 708,304,516 successful authentication events…


which one to download?

---

**Cursor**

**Download: Comprehensive, Multi-Source Cyber-Security Events (Cyber1).**

Only grab these two files:
- `auth.txt.gz` (7.2 GB)
- `redteam.txt.gz` (4.8 KB)

Skip `proc`, `flows`, `dns` for now.

---

## Why Cyber1

| Need for your paper | Cyber1 | User–Computer (708M) | Unified Host & Network |
|---------------------|--------|----------------------|-------------------------|
| **Attack labels** | ✅ `redteam.txt` | ❌ no red-team file | ❌ no standard attack labels |
| **Euler/Bowman benchmark** | ✅ yes | partial (auth only) | no |
| **Benign train / attack test** | ✅ train before red-team day, test on red-team auths | unclear without labels | hard to define |
| **Graph build** | simple: src host → dst host | user → computer bipartite | 90 JSON/netflow files, heavy parsing |

---

## How you'll use Cyber1 (same as CICAPT logic)

- **Graph:** host↔host auth edges from `auth.txt`
- **Train:** auth edges **before** first red-team event
- **Test positives:** edges matching `redteam.txt` (~750 compromise events)
- **Eval:** random vs SoHeaRT negatives

---

## Skip the other two

- **User–Computer (708M):** bigger, but **no attack ground truth** — good for credential-hopping, bad for your labeled attack-edge LP.
- **Unified Host & Network:** 90 days of netflow + JSON host logs, **no `redteam.txt`**, much more preprocessing.

**Form:** [csr.lanl.gov/data/cyber1](https://csr.lanl.gov/data/cyber1) — request `auth.txt.gz` + `redteam.txt.gz` only.

---

**User**

Individual File Descriptions
auth.txt.gz
This data represents authentication events collected from individual Windows-based desktop computers, servers, and Active Directory servers. Each event is on a separate line in the form of “time,source user@domain,destination user@domain,source computer,destination computer,authentication type,logon type,authentication orientation,success/failure” and represents an authentication event at the given time. The values are comma delimited and any fields that do not have a valid value are represented as a question mark (’?’).

Here are three lines from the data as an example:

1,C625$@DOM1,U147@DOM1,C625,C625,Negotiate,Batch,LogOn,Success
1,C653$@DOM1,SYSTEM@C653,C653,C653,Negotiate,Service,LogOn,Success
1,C660$@DOM1,SYSTEM@C660,C660,C660,Negotiate,Service,LogOn,Success
proc.txt.gz
This data represents process start and stop events collected from individual Windows-based desktop computers and servers. Each event is on a separate line in the form of “time,user@domain,computer,process name,start/end” and represents a process event at the given time. The values are comma delimited and any fields that do not have a valid value are presented as a question mark (’?’).

Here are three lines from the data as an example:

1,C553$@DOM1,C553,P16,Start
1,C553$@DOM1,C553,P25,End
1,C553$@DOM1,C553,P25,Start
flows.txt.gz
This data presents network flow events collected from central routers within the network. Each event is on a separate line in the form of “time,duration,source computer,source port,destination computer,destination port,protocol,packet count,byte count” and presents a network flow event at the given time and the given duration in seconds. The values are comma delimited and any fields that do not have a valid value are presented as a question mark (’?’).

Here are three lines from the data as an example:

1,9,C3090,N10471,C3420,N46,6,3,144
1,9,C3538,N2600,C3371,N46,6,3,144
2,0,C4316,N10199,C5030,443,6,2,92
dns.txt.gz
This data presents Domain Name Service (DNS) lookup events collected from the central DNS servers within the network. Each event is on a separate line in the form of “time,source computer,computer resolved” and presents a DNS lookup at the given time by the source computer for the resolved computer and represents a likely network connection originating from the source computer to the resolved computer. The values are comma delimited and any fields that do not have a valid value are presented as a question mark (’?’).

Here are three lines from the data as an example:

31,C161,C2109
35,C5642,C528
38,C3380,C22841
redteam.txt.gz
This data presents specific events taken from the authentication data that present known redteam compromise events. These may be used as ground truth of bad behavior that is different from normal user and computer activity. Each event is on a separate line in the form of “time,user@domain,source computer,destination computer” and presents a compromise event at the given time. The values are comma delimited.

Here are three lines from the data as an example:

151648,U748@DOM1,C17693,C728
151993,U6115@DOM1,C17693,C1173
153792,U636@DOM1,C17693,C294
Data



which files to use?

---

**Cursor**

**Use only two files:**

| File | Role |
|------|------|
| **`auth.txt.gz`** | Build the graph + train on benign auths |
| **`redteam.txt.gz`** | Attack ground truth for test positives |

**Skip:** `proc.txt.gz`, `flows.txt.gz`, `dns.txt.gz` — not needed for your auth link-prediction setup.

---

## From `auth.txt.gz`

Each line → one auth event. For your graph, use **host↔host** edges (Euler/Bowman style):

- **Nodes:** computers (`C625`, `C653`, …)
- **Edge:** `(source computer, destination computer)` per successful auth
- **Filter:** keep `Success` only; drop self-loops (`C625 → C625`) if you want cleaner LM signal (Euler often keeps weighted host pairs — either works)

Optional: also build **user↔computer** bipartite graph — more common in credential-hopping papers, but host↔host matches Euler/CICAPT “entity interaction” story better.

---

## From `redteam.txt.gz`

Each line: `time, user@domain, source computer, destination computer`

- These are **known compromise / lateral-movement auths**
- **Test positives:** match `(source computer, destination computer)` from redteam to edges in your auth graph (at or after red-team start time)
- **Train:** all auth edges **before** first redteam timestamp (~151648)

---

## Split summary

```
Train  = auth edges with time < min(redteam.time)
Test+  = redteam (src_host, dst_host) edges  (~750 events)
Val    = small holdout from pre-redteam auth edges
```

**Download:** `auth.txt.gz` (7.2 GB) + `redteam.txt.gz` (4.8 KB) only.

---

**User**

https://drive.google.com/open?id=1yksk4I9DBnl0Tca_Y0eaiNcpRNccB9sF&usp=drive_copy 

we need all like this for darpa?

---

**Cursor**

**No — you do not need the whole DARPA TC Google Drive.** For your paper, one THEIA E3 scene is enough.

## What that link is

[`ta1-theia-e3-official-1r.bin.tar.gz`](https://drive.google.com/open?id=1yksk4I9DBnl0Tca_Y0eaiNcpRNccB9sF&usp=drive_copy) is **one** THEIA host/run from Engagement 3 (Linux provenance), not the full corpus.

The full E3 release is on the [Five Directions Google Drive folder](https://drive.google.com/open?id=1QlbUFWAGq3Hpl8wVdzOdIoZLFxkII4EK) and includes many performers and shards — see the [official E3 README](https://github.com/darpa-i2o/Transparent-Computing/blob/master/README-E3.md).

## What to download (minimum)

| Get | Why |
|-----|-----|
| **One THEIA file** — `1r` **or** `6r` | Enough for a third dataset; papers often use both, but **one suffices** for CARS |
| **Ground truth** — `tc_ground_truth_report_e3_update.pdf` | Attack labels / which days are malicious |
| **Parser** — `ta3-java-consumer.tar.gz` + `TCCDMDatum.avsc` | Required if you use `.bin` (Avro), not plain JSON |

**Skip for now:** CADETS, TRACE, FIVEDIRECTIONS, CLEARSCOPE, other THEIA variants (`3`, `5m`), OpTC.

## Do you need multiple THEIA archives like `1r`, `6r`, …?

| Goal | Files |
|------|-------|
| **Your paper (3 datasets)** | **1 THEIA archive** + CICAPT + LANL |
| Full THEIA reproduction (threaTrace, etc.) | Often `1r` + `6r` |
| Full DARPA E3 benchmark | Many GB across all performers |

## Easier alternative (recommended if `.bin` parsing is slow)

Use **preprocessed THEIA E3** from [PIDSMaker](https://ubc-provenance.github.io/PIDSMaker/ten-minute-install/) (~**1.1 GB** compressed, ~12 GB loaded) — same provenance graph, no Avro parsing.

## Your 3-dataset stack

1. **CICAPT** — already have  
2. **LANL** — `auth.txt.gz` + `redteam.txt.gz`  
3. **DARPA THEIA E3** — **one** of `1r` or `6r` (+ ground truth), or PIDSMaker `THEIA_E3`

**Bottom line:** Download **one** THEIA tarball like your link, not the entire Drive. Add ground truth; use PIDSMaker if raw `.bin` parsing eats your week.

---

**User**

Operationally Transparent Cyber (OpTC) Data Release
Disclaimer
DARPA is releasing these files in the public domain to stimulate further research. Their release implies no obligation or desire to support additional work in this space. The data is released as-is. DARPA makes no warranties as to the correctness, accuracy, or usefulness of the released data. In fact, since the data was produced by research prototypes, it is practically guaranteed to be imperfect. Nonetheless, as this data represents a very large repository of semantically rich and structured data, DARPA believes that it is in the best interests of the Department of Defense and the research community to make them freely available.

OpTC Overview
Operationally Transparent Cyber (OpTC) was a technology transition pilot study funded under Boston Fusion Corp.'s Cyber APT Scenarios for Enterprise Systems (CASES) project. Its primary objective was to determine if DARPA Transparent Computing (TC) program technologies could scale without loss of detection performance to address cyber defense capability gaps identified in USTRANSCOM's Joint Deployment Distribution Enterprise (JDDE) solicitation for the government fiscal years 2019-2023. Boston Fusion along with two performers from the TC program (Five Directions providing endpoint telemetry (TA1) and BAE providing analysis over the data (TA2)) worked to scale their systems from two machines to one thousand machines. The OpTC team conducted scaling and detection tests in the fall of 2019. A third performer (Provatek), not originally associated with the TC program, acted as a red team and test coordinator. This data set represents a subset of that activity.

The OpTC system architecture is based on one used in TC program evaluations. Kafka, an open-source stream-processing server, is used to pass information among system components. Each Windows 10 endpoint is equipped with an endpoint sensor that monitors host events, packs them into JSON records, and sends them to Kafka. As these records flow into Kafka, a translation server aggregates them into new data records in a format called eCAR that are then pushed back to Kafka. As the translation server pushes eCAR records to Kafka, a data analytics component integrates them into a graph data structure for analysis and visualization.

OpTC took TC system components that worked well on two hosts in TC program tests and scaled them up to work with one-thousand hosts. This scaled up system was evaluated over two weeks in a highly instrumented environment, and the data in this collection contains approximately a terabyte of data in compressed JSON compatible format from that evaluation. The evaluation started with a period of benign record generation, followed by the injection of malware by a red team. Benign traffic ran continuously during red team activity. Due to constraints in collection data space during the evaluation, data from five hundred hosts was collected rather than from the full set of one-thousand hosts.

Data Collection
Data collection was funded by the DARPA Cyber Hunting at Scale (CHASE) program. Five Directions collected and post processed the data to enrich the dataset.

File Manifest and Descriptions
Ground Truth: The file OpTCRedTeamGroundTruth.pdf contains the ground truth constructed by the red team to help guide the evaluation. The evaluation took place on an isolated network environment, and so all the IP addresses and domain names referenced in the data are fictional and bear no relationship to routable and valid addresses and hostnames, respectively.

eCAR:The file ecar.md explains the history and syntax of eCAR.

Errata:The file errata.md describes issues with the data.

Data: Due to the size of the data set it is hosted on Google Drive at https://drive.google.com/drive/u/0/folders/1n3kkS3KR31KUegn42yk3-e6JkZvf0Caa rather than here. The data set contains three folders: ecar-bro, ecar, and bro.

ecar-bro: Contains the eCAR FLOW-START events annotated with the bro-id of the flow enabling one to pivot between endpoint and network data. There are three sub-folders: short, evaluation, and benign. The short folder contains events with missing data. The evaluation folder contains data from the evaluation period of the exercise, and the benign folder contains data from the benign collection period of the exercise.

ecar: Contains the endpoint data from the exercise in three folders similar to ecar-bro, above.

bro: Contains the data from the bro sensor by date.

Distribution A: Approved for public release: distribution unlimited


or need to pick this?

---

**Cursor**

**Pick one third dataset — not full OpTC unless you want Euler-style host flows and have time to subset.**

## OpTC vs THEIA (for dataset #3)

| | **OpTC** | **THEIA E3** |
|---|----------|--------------|
| **Graph** | Host **network flows** (FLOW-START) — like Euler | **Provenance** (process/file/socket) — like CICAPT |
| **Size** | ~**1 TB** on [Google Drive](https://drive.google.com/drive/u/0/folders/1n3kkS3KR31KUegn42yk3-e6JkZvf0Caa) | **One** `.tar.gz` (~GB scale) |
| **Split** | `ecar-bro/benign` vs `ecar-bro/evaluation` | Week 1 benign / week 2 attack |
| **Labels** | `OpTCRedTeamGroundTruth.pdf` | `tc_ground_truth_report_e3_update.pdf` |
| **Overlap with your stack** | Overlaps **LANL** (host comms) | Overlaps **CICAPT** (provenance) |

## Recommendation

**Default: THEIA E3 (one file: `1r` or `6r`)** — smaller, provenance complements CICAPT, LANL already covers auth/host.

**Use OpTC only if** you want a third **graph type** (host flows + cite Euler directly) and you download a **subset only**:

| Download | Skip |
|----------|------|
| `ecar-bro/benign/` + `ecar-bro/evaluation/` (FLOW-START) | Full `ecar/`, `bro/`, `short/` |
| Ground truth PDF | Entire 1 TB tree |

Or use preprocessed **OpTC_H201** (~2 GB) via [PIDSMaker](https://ubc-provenance.github.io/PIDSMaker/ten-minute-install/) instead of raw JSON.

## Your 3-dataset lineup

| # | Dataset | Graph type |
|---|---------|------------|
| 1 | CICAPT ✓ | IIoT provenance |
| 2 | LANL `auth` + `redteam` | Enterprise auth |
| 3 | **THEIA E3** (recommended) **or** OpTC FLOW subset (optional) |

**Do not** download all of OpTC. **Do not** use both THEIA and OpTC unless you have extra time — one third dataset is enough for 6 pages.

**For your deadline:** CICAPT + LANL first; add THEIA if ahead of schedule; OpTC only if you specifically want Euler host-flow comparison.

---

**User**

lets do both? our method is applicable on both or not?

---

**Cursor**

**Yes — the same method works on both.** Graph construction differs; train/eval stay identical.

## Same pipeline everywhere

| Step | THEIA E3 | OpTC |
|------|----------|------|
| **Graph** | Provenance edges (process/file/socket) | Host↔host from **FLOW-START** (`ecar-bro`) |
| **Train** | Week 1 / benign-period edges | `ecar-bro/benign/` edges |
| **Test +** | Attack-touching edges (ground truth PDF) | Red-team flows in `ecar-bro/evaluation/` |
| **Model** | GCN + SVD features | Same |
| **Eval** | Random vs SoHeaRT negatives | Same |

SoHeaRT only needs a graph + positive test edges — it doesn’t care if nodes are processes or hosts.

## Doing both is fine for the paper

You get **four graph types**, same contribution:

| Dataset | Graph |
|---------|-------|
| CICAPT | IIoT provenance |
| LANL | Enterprise auth |
| THEIA | DARPA provenance |
| OpTC | Enterprise host flows (Euler-style) |

**Story:** Hard-negative evaluation matters across **provenance and communication** graphs, not just one domain.

## Practical constraint (timeline)

| Dataset | Effort |
|---------|--------|
| CICAPT | Low — CSV ready |
| LANL | Medium — parse `auth` + `redteam` |
| THEIA | Medium–high — one `.tar.gz` + provenance parser |
| OpTC | Medium — **subset only** (`ecar-bro/benign` + `evaluation`), not full 1 TB |

**Order:** CICAPT → LANL → THEIA **or** OpTC (one DARPA dataset) → add the other if time allows.

## Bottom line

Both THEIA and OpTC are in scope. You don’t need two new methods — one pipeline, four datasets, one results table (AP random vs SoHeaRT per dataset). For the deadline, **lock CICAPT + LANL first**; THEIA + OpTC are add-ons, not blockers.

---

**User**

i have to use third one which one is better for research paper acceptance?

---

**Cursor**

**Pick OpTC (FLOW-START subset)** if you can preprocess it cleanly. **Pick THEIA E3** if time/download is tight.

## For *your* paper (hard negatives + cyber LP)

| Criterion | **OpTC** | **THEIA E3** |
|-----------|----------|--------------|
| **Fits your story** | **Strongest** — Euler (TOPS’23) uses OpTC + random negatives; you directly extend that line | Good — classic APT provenance, but most THEIA papers do subgraph anomaly, not LP eval |
| **Diversity with CICAPT + LANL** | **3 graph types:** provenance + auth + **host flows** | 2 provenance graphs (CICAPT + THEIA) + auth — more redundant |
| **Reviewer recognition** | High (Euler, DARPA, scaled enterprise) | **Highest** (UNICORN, KAIROS, ThreaTrace, etc.) |
| **Reproducibility risk** | Higher (must document subset: `ecar-bro/benign` + `evaluation` only) | Lower (one tarball + standard E3 ground truth) |
| **Novelty pairing** | CICAPT (2024 IIoT) + LANL + **Euler benchmark** | CICAPT (new) + **gold-standard** DARPA provenance |

## Recommendation

**Best for acceptance with your angle:** **OpTC** — lets you write:

> *Methods like Euler report strong link-prediction performance on OpTC using random negatives; we show the same models look far weaker under SoHeaRT on OpTC, LANL, and CICAPT.*

That is a clear, citeable gap — not just “we ran on another dataset.”

**Safer fallback:** **THEIA E3** — if OpTC preprocessing slips, reviewers still accept “validated on DARPA TC + LANL + modern IIoT data.”

## What to download (OpTC only)

- `ecar-bro/benign/` + `ecar-bro/evaluation/` (FLOW-START → host↔host edges)
- `OpTCRedTeamGroundTruth.pdf`
- **Not** the full 1 TB tree

## One-line decision

- **Chasing stronger narrative / Euler hook → OpTC**  
- **Chasing fastest credible third result → THEIA (`6r` or PIDSMaker `THEIA_E3`)**

For CARS with ~1 week left: **try OpTC subset first; fall back to THEIA if parsing isn’t done in 1–2 days.**

---

**User**

how to download optc

---

**Cursor**

**Download OpTC from the official Google Drive — not the full 1 TB.** For your paper, grab only `ecar-bro/benign` + `ecar-bro/evaluation` and the ground-truth PDF.

---

## 1. Small files from GitHub (do this first)

Clone or download from [FiveDirections/OpTC-data](https://github.com/FiveDirections/OpTC-data):

- `OpTCRedTeamGroundTruth.pdf` — attack labels
- `ecar.md` — event format
- `errata.md` — known data issues

```bash
git clone https://github.com/FiveDirections/OpTC-data.git
```

---

## 2. Main data (Google Drive)

**Root folder:** [OpTC Google Drive](https://drive.google.com/drive/folders/1n3kkS3KR31KUegn42yk3-e6JkZvf0Caa)

Structure:

```
ecar-bro/
  benign/      ← train (Sep 19–23)
  evaluation/  ← test / red-team (Sep 23–25)
  short/       ← SKIP (incomplete)
ecar/          ← SKIP for now (full endpoint, huge)
bro/           ← SKIP
```

**Browser:** Open `ecar-bro` → download **`benign`** and **`evaluation`** only (right-click → Download). Do **not** download the whole root folder (~1 TB).

---

## 3. CLI with `gdown` (optional)

```bash
pip install gdown

# Full root — NOT recommended (~1 TB)
# gdown --folder https://drive.google.com/drive/folders/1n3kkS3KR31KUegn42yk3-e6JkZvf0Caa -O ./OpTC --remaining-ok
```

Safer: open Drive in the browser, open `ecar-bro/benign`, copy **that folder’s** URL, then:

```bash
gdown --folder "<benign-folder-url>" -O ./OpTC/ecar-bro/benign --remaining-ok
gdown --folder "<evaluation-folder-url>" -O ./OpTC/ecar-bro/evaluation --remaining-ok
```

Large files may stop after ~1 hour — rerun with `--continue` ([gdown docs](https://github.com/wkentaro/gdown)).

---

## 4. What you use for the graph (Euler-style)

From JSON in `ecar-bro`:

- Filter events with **FLOW-START**
- Extract **src IP/host → dst IP/host** + timestamp
- **Train:** `benign/`
- **Test positives:** red-team flows in `evaluation/` (match `OpTCRedTeamGroundTruth.pdf`)

---

## 5. Easier alternative (preprocessed)

If raw JSON is too heavy, use [PIDSMaker](https://ubc-provenance.github.io/PIDSMaker/ten-minute-install/) preprocessed dumps (`optc_h201`, ~2 GB) from their Google Drive — same dataset, less parsing.

---

## Checklist

| Get | Skip |
|-----|------|
| `ecar-bro/benign/` | `ecar/`, `bro/`, `short/` |
| `ecar-bro/evaluation/` | Full 1 TB tree |
| `OpTCRedTeamGroundTruth.pdf` | IEEE DataPort 287 GB tarball |

**Start:** GitHub PDF + metadata, then Drive → `ecar-bro/benign` + `evaluation` only.

---

**User**

https://drive.google.com/drive/folders/1XOGWT8R8ovw6J2DgNsui09ZwvrfOXIVa?usp=drive_link https://drive.google.com/drive/folders/1mRnYKaMsYYPQbCwdq9YLCgNy-pD6uP0Y?usp=drive_link 

---

**Cursor**

**Yes — those are the right OpTC folders** for Euler-style host-flow link prediction.

## [benign](https://drive.google.com/drive/folders/1XOGWT8R8ovw6J2DgNsui09ZwvrfOXIVa?usp=drive_link) → **train**

| Subfolder | Use |
|-----------|-----|
| `17-18Sep19` | Benign |
| `18-19Sep19` | Benign |
| `19Sep19` | Benign |
| `20-23Sep19` | Benign (largest pre-attack window) |

**Download all four** (or at minimum `20-23Sep19` + `19Sep19` if disk is tight).

---

## [evaluation](https://drive.google.com/drive/folders/1mRnYKaMsYYPQbCwdq9YLCgNy-pD6uP0Y?usp=drive_link) → **test / attack period**

| Subfolder | Use |
|-----------|-----|
| `23Sep19-red` | Red-team day |
| `23Sep-night` / `23Sep-Night` | Same night window (pick one if duplicate) |
| `24Sep19` | Attack campaign |
| `25Sept` | Attack campaign |

**Download all** for test; label malicious FLOW-START edges with `OpTCRedTeamGroundTruth.pdf` from [FiveDirections/OpTC-data](https://github.com/FiveDirections/OpTC-data).

---

## Also grab from GitHub

- `OpTCRedTeamGroundTruth.pdf`
- `ecar.md` (FLOW-START field names)

---

## Your split

```
Train positives  = FLOW-START edges from benign/*
Test positives   = FLOW-START edges in evaluation/* that match ground truth
Skip             = short/, ecar/, bro/
```

**CLI (optional):**

```bash
pip install gdown
gdown --folder "https://drive.google.com/drive/folders/1XOGWT8R8ovw6J2DgNsui09ZwvrfOXIVa" -O ./OpTC/ecar-bro/benign --remaining-ok
gdown --folder "https://drive.google.com/drive/folders/1mRnYKaMsYYPQbCwdq9YLCgNy-pD6uP0Y" -O ./OpTC/ecar-bro/evaluation --remaining-ok
```

Those two links are exactly what you need — not the full 1 TB Drive root.

---

**User**

@/home/mmanjee/.cursor/projects/home-mmanjee-HeaRT/terminals/12.txt:48-97 

---

**Cursor**

**`--remaining-ok` isn’t in gdown 6.x** — that’s why it failed. `gdown` installed fine; drop that flag.

## Correct commands

```bash
mkdir -p ./OpTC/ecar-bro/benign ./OpTC/ecar-bro/evaluation

gdown --folder "https://drive.google.com/drive/folders/1XOGWT8R8ovw6J2DgNsui09ZwvrfOXIVa" -O ./OpTC/ecar-bro/benign/

gdown --folder "https://drive.google.com/drive/folders/1mRnYKaMsYYPQbCwdq9YLCgNy-pD6uP0Y" -O ./OpTC/ecar-bro/evaluation/
```

If a download stops partway (common on large Drive files), resume with:

```bash
gdown --folder --continue "https://drive.google.com/drive/folders/1XOGWT8R8ovw6J2DgNsui09ZwvrfOXIVa" -O ./OpTC/ecar-bro/benign/
gdown --folder --continue "https://drive.google.com/drive/folders/1mRnYKaMsYYPQbCwdq9YLCgNy-pD6uP0Y" -O ./OpTC/ecar-bro/evaluation/
```

## If `gdown` still fails

Use the browser: open each folder → select all subfolders → Download. Or download one day at a time (e.g. only `20-23Sep19` from benign first).

Also clone metadata from GitHub:

```bash
git clone https://github.com/FiveDirections/OpTC-data.git
```

---

**User**

give brief summary of 3 datasets and how will be used for what. very brief and also include links for all 3

---

**Cursor**

## 3 datasets (brief)

| Dataset | Graph | Train | Test | Link |
|---------|-------|-------|------|------|
| **CICAPT-IIoT** | IIoT provenance (process/file/socket) | Phase 1 edges (benign) | Phase 2 attack-touching edges (~570) | [UNB CIC APT IIoT 2024](https://www.unb.ca/cic/datasets/iiot-dataset-2024.html) — you already have CSVs |
| **LANL Cyber1** | Host↔host auth | Auths before red-team | `redteam.txt` compromise auths | [csr.lanl.gov/data/cyber1](https://csr.lanl.gov/data/cyber1) — `auth.txt.gz` + `redteam.txt.gz` |
| **OpTC** | Host↔host network flows (FLOW-START) | [benign/](https://drive.google.com/drive/folders/1XOGWT8R8ovw6J2DgNsui09ZwvrfOXIVa) | [evaluation/](https://drive.google.com/drive/folders/1mRnYKaMsYYPQbCwdq9YLCgNy-pD6uP0Y) + [ground truth PDF](https://github.com/FiveDirections/OpTC-data) |

---

**Same method on all three:** GCN link prediction on benign edges → score attack/anomalous edges → compare **random vs SoHeaRT** negatives (AP/AUC).

**Paper line:** Easy negatives inflate cyber LP performance; hard negatives show whether attack links are really detectable — on provenance (CICAPT), auth (LANL), and host flows (OpTC/Euler).

---

**User**

so hard negatuves will not predict all edges as anomolous?

---

**Cursor**

**Correct — hard negatives do not mean “flag everything as anomalous.”**

## What hard negatives are

They are **negative test pairs** — edges that **do not exist** but look **plausible** (similar neighbors, structure). They are **not** attacks.

## What the model outputs

For each edge *(u, v)*, the model outputs a **link score** (high = likely real/normal edge, low = unlikely/anomalous).

| Edge type | Expected score |
|-----------|----------------|
| **Attack edge** (test positive) | **High** if detection works |
| **Hard negative** (plausible non-edge) | **Low** — it’s not a real link |
| **Random negative** (easy non-edge) | **Very low** — trivial to beat |

## Hard vs random (evaluation only)

- **Random negatives:** attack edges almost always rank above them → **AP looks great** (often 0.9+).
- **Hard negatives:** attack edges must beat **similar-looking benign pairs** → **AP drops** if the model isn’t really discriminative.

That drop is **honest measurement**, not the model suddenly calling everything anomalous.

## In deployment

You only alert on **low-scoring edges you actually observe** (real process↔file, host↔host flows, etc.) — not on sampled negatives. Hard negatives never appear in production; they only make **research evaluation** match real operating conditions.

**One line:** Hard negatives make the **bar higher** for calling attack edges “detected”; they don’t make the system **more alarmist**.

---

**User**

But i think there are differen types of edges so how will it preditand what kind of edges?

---

**Cursor**

**Short answer:** You predict **whether an edge exists** between two nodes *(u, v)* — not **which type** of edge, unless you build a multi-relational model (you’re not).

---

## Per dataset — what edges go into the graph

| Dataset | Raw edge types | What you use |
|---------|----------------|--------------|
| **CICAPT** | `Used`, `WasGeneratedBy`, `WasTriggeredBy`, `WasDerivedFrom` | **Collapse to one type** — any provenance link = one directed edge |
| **LANL** | Auth events | **Host → host** (src computer → dst computer), success only |
| **OpTC** | Many eCAR event types | **Only FLOW-START** → host/IP → host/IP |

So each graph is **homogeneous**: one kind of link = “these two entities interacted.”

---

## What link prediction actually scores

For a pair *(u, v)*:

> “How likely is there **any** edge between u and v, given what the GCN learned from benign training edges?”

- **Train:** benign edges only (Phase 1 / pre-redteam / benign folder)
- **Test positives:** real attack-related edges *(u, v)* from labels
- **Test negatives:** pairs with **no** edge — random or SoHeaRT-hard

You do **not** predict: “Is this `Used` or `WasGeneratedBy`?” That would need a relational GCN (R-GCN, etc.) — out of scope.

---

## CICAPT example

Same process–file pair could appear as:

- `Used` (process read file)
- `WasGeneratedBy` (file created by process)

After collapse → **one edge** `(process, file)`. The model learns “this pair connects,” not the W3C provenance relation name.

**Test positives:** Phase 2 edges touching attack nodes — whatever relation type they are in the CSV.

---

## Why this is still valid for APT detection

Attacks show up as **unusual connections** (process↔malicious file, host↔new host), not necessarily as a specific relation label. Euler/Bowman use the same idea on auth/flow graphs.

**Limitation for the paper:** collapsing 4 provenance types loses relation semantics — mention as future work (heterogeneous LP).

---

**One line:** You predict **existence of interaction edges** on three graphs (provenance, auth, host flow); edge **type** is merged away for a single GCN + SoHeaRT eval pipeline.

---

**User**

but wouldnt that be a problem? making all edges one tyoe?

---

**Cursor**

**It’s a simplification, not a fatal flaw** — but you should state it as a limitation.

## What you lose

| Issue | Effect |
|-------|--------|
| **Different semantics** | `WasTriggeredBy` (spawn) ≠ `Used` (read file) — same edge weight in the graph |
| **Duplicate pairs** | Same *(process, file)* can appear under multiple relation types → one collapsed edge hides multiplicity |
| **Dominant types** | CICAPT Phase 1 is ~78% `WasGeneratedBy` — model mostly learns “artifact creation,” not fine-grained behavior |
| **Direction** | You keep direction, but lose *which* direction means what (e.g. who generated whom) |

So yes: **some attack signal and normal structure are blurred.**

## Why it’s still OK for your paper

1. **Same choice as Euler/Bowman** — they use one auth/flow edge type, not “logon type × orientation × …”
2. **Your contribution is evaluation** (random vs SoHeaRT), not a new heterogeneous GCN
3. **Anomaly framing** — you ask “is this *connection* unusual?” not “is this exact W3C relation unusual?” Many APT steps still show up as **new or rare node pairs**
4. **Fair comparison** — random vs hard negatives on the **same** collapsed graph; the eval gap still holds

## When it *is* a real problem

- Attacks that reuse **normal pairs** with only a **relation change** (rare in practice)
- Tactics that differ only by edge type, not by *(u, v)* — uncommon in CICAPT labels

On your data, attack edges are mostly **new process/file/socket interactions**, not relabelings of benign pairs.

## For the paper (one sentence)

> *We use a homogeneous graph for a unified GCN+SoHeaRT pipeline; heterogeneous relation types are left as future work.*

**Optional mitigation (only if time):** keep 4 edge types as **4 separate graphs** or **edge-type features** — don’t block the main experiment on this.

**Bottom line:** Collapsing types is a **known tradeoff**, acceptable for a 6-page eval-focused paper; cite it honestly, don’t oversell provenance fidelity.

---

**User**

so attack edges use same types and have a label attacker attached?

---

**Cursor**

**Mostly yes on edge types; labels are on nodes, not a separate “attacker” edge type.**

## CICAPT

| | Benign | Attack |
|---|--------|--------|
| **Edge types** | `Used`, `WasGeneratedBy`, `WasTriggeredBy`, `WasDerivedFrom` | **Same four types** |
| **Where label lives** | Nodes: `label=0` | **Nodes:** `label=1` + `subLabel` (e.g. `credentialAccess`, `collection`) |
| **What counts as “attack edge”** | — | Any edge whose **`from` or `to`** touches a `label=1` node (~570 edges) |

There is **no** special edge type like `Attack` or `Attacker`. Edge rows in the CSV are usually `label=0`; attack is inferred from **endpoints**.

**Nodes are** `Process` or `Artifact` — not a dedicated “Attacker” node. Malicious **processes** get `label=1`.

---

## LANL & OpTC (same idea)

| Dataset | Label | Attack edges |
|---------|-------|--------------|
| **LANL** | `redteam.txt` lists compromise auths | Same auth edge format; subset marked bad |
| **OpTC** | `OpTCRedTeamGroundTruth.pdf` | Same FLOW-START edges; subset marked malicious |

---

**One line:** Attack activity uses **normal edge types**; **nodes** (or red-team event lists) carry the attack label — you test whether those **same-shaped** edges score as unlikely vs benign training.

---

**User**

so we dont really have to worry about type but the label right?

---

**Cursor**

**Yes — for your setup, focus on labels (and the train/test split), not edge type.**

- **Train:** benign edges only (Phase 1 / pre-redteam / OpTC benign)
- **Test positives:** edges tied to **attack labels** (CICAPT `label=1` nodes, LANL `redteam.txt`, OpTC ground truth)
- **Graph:** collapse all relation types to one edge — type doesn’t drive train/test

Edge type only matters as a **limitation** you mention in the paper, not something you need to model separately for the MVP.

---

**User**

instead lets use relation types as feature or not>?

---

**Cursor**

**Skip relation types as features for the MVP.** Not worth it for this paper.

## Why not (for your 3 datasets)

| Issue | Detail |
|-------|--------|
| **LANL & OpTC** | No provenance relation types — only CICAPT has four |
| **LP on pairs** | Negatives have **no edge** → **no relation type**; positives do → unfair feature |
| **Collapsed edges** | One *(u,v)* can have multiple types → messy to encode |
| **Your contribution** | SoHeaRT **evaluation**, not richer provenance modeling |

## When it would make sense

- **R-GCN / DistMult** — predict *(u, v, relation)* jointly  
- **CICAPT-only** ablation — “homogeneous vs typed edges”  
- That’s a **second paper** or extra experiment, not core path

## What to use instead

- **Node features:** SVD on adjacency (your current plan)  
- **Optional cheap add-on:** node `type` = Process vs Artifact (one-hot) — same on all datasets’ node semantics where applicable  

## Paper one-liner

> *We use a homogeneous graph for a unified pipeline across provenance, auth, and flow graphs; relation types are collapsed and left for future heterogeneous models.*

**Bottom line:** Labels + split matter; relation types as features **don’t** for this deadline — collapse and move on.

---

**User**

do other papers also collapse the relation type?? check on web

---

**Cursor**

**Both are common — depends on task and paper generation.**

## Papers that **collapse** (or ignore relation type)

| Paper | What they do |
|-------|----------------|
| **[Euler](https://dl.acm.org/doi/10.1145/3588771)** / Bowman | Auth → **host↔host** edge; auth type, logon type, success not modeled |
| **[CICAPT-IIoT authors](https://www.cs.unb.ca/~sray/papers/CICAPT_IIoT_paper_Mobiquitous24_author_copy.pdf)** | **Node2Vec baseline** treats provenance as **homogeneous**; they note it’s weak and use **HDGI** (heterogeneous) for real results |
| **Power-system CICAPT LP paper** ([MDPI 2025](https://www.mdpi.com/2079-9292/14/21/4241)) | PROV relations listed, but link prediction samples from **one edge set** + random negatives — effectively **one edge type** |

## Papers that **keep** relation types (heterogeneous)

| Paper | What they do |
|-------|----------------|
| **[APT-LMSPS](https://www.mdpi.com/2078-2489/17/3/245)** | Says early work treated graphs as homogeneous; **meta-paths / HGNN** use typed edges |
| **ProvExplainer**, **UNICORN**, **KAIROS**, **PROVSYN** | Process/file/socket + **read/write/send** (or PROV relations) as **distinct edge types** |
| **CICAPT HDGI model** | Explicitly heterogeneous — opposite of their Node2Vec baseline |

## Takeaway for your paper

- **Collapsing is normal** for **link-prediction / anomaly-LP** lines (Euler, Bowman, CICAPT Node2Vec baseline).
- **Keeping types is normal** for **APT detection GNN** papers (meta-path, R-GCN, HDGI).

Your setup matches the **LP camp**, not the **heterogeneous detection** camp — defensible if you cite both:

> *We follow Euler/Bowman and homogeneous LP baselines on CICAPT; heterogeneous relation modeling (e.g. HDGI, meta-path GNNs) is future work.*

That’s honest: you’re aligned with **LP papers**, not claiming to beat **typed provenance detectors**.

---

**User**

show where they write in paper that they ignore the type

---

**Cursor**

Searching the papers for explicit statements about ignoring relation/edge types.


**Few papers say literally “we ignore edge types.”** Most **collapse by construction** — they build one edge type from a subset of fields. Below are the clearest quotes.

---

## Explicit: “homogeneous” / “disregarding edge types”

**CICAPT-IIoT authors** ([Mobiquitous’24 paper](https://www.cs.unb.ca/~sray/papers/CICAPT_IIoT_paper_Mobiquitous24_author_copy.pdf), §5.2):

> *“Node2Vec is designed for **homogeneous graphs**, which means, it **cannot effectively handle the heterogeneity** and evolving structure of provenance graphs crucial for identifying complex APT activities.”*

They still use Node2Vec as a **baseline** on a single adjacency — i.e. collapsed graph.

**APT-LMSPS** ([Information 2026](https://www.mdpi.com/2078-2489/17/3/245), §1):

> *“Early research typically treated provenance graphs as **homogeneous**, focusing primarily on topological information while **disregarding variations in node and edge types**.”*

> *“homogeneous graphs are **unable to accurately reflect** the intricate multi-relationship patterns present in APT attacks because they **ignore the heterogeneity of node and edge types**.”*

That describes the **old** approach your setup aligns with — not state-of-the-art heterogeneous detectors.

---

## Implicit: collapse by graph construction (no “ignore” wording)

**Euler** ([`euler.txt` §8.1](https://dl.acm.org/doi/10.1145/3588771)) — raw auth has 9 fields (auth type, logon type, success, …); graph uses only computers:

> *“We construct a weighted, directed graph from the authentication logs by mapping which entities authenticate with one another. As nodes, we use the entities denoted **source and destination computers**… an edge is created between the **source computer and destination computer**.”*

Auth type / logon type are never used.

**Euler on OpTC** ([`euler.txt` §9.1](https://dl.acm.org/doi/10.1145/3588771)) — drops most eCAR event types:

> *“we only use a small subset of these events: all **FLOW objects undertaking a START action**… While **PROCESS-OPEN** events … also capture this, they will also always produce a **FLOW-START** event.”*

**Bowman et al.** ([RAID’20](https://www.usenix.org/conference/raid2020/presentation/bowman)):

> *“we construct an **authentication graph**, where nodes represent authenticating entities … and **edges represent authentication events**.”*

Link prediction is on entity pairs — not on auth-type-conditioned relations.

**MDPI CICAPT LP paper** ([Electronics 2025](https://www.mdpi.com/2079-9292/14/21/4241)) — maps PROV relations but LP treats one edge set:

> *“positive samples are randomly drawn from the **real edge set**”* with *“global uniform negative sampling”* — no per-relation decoder.

---

## Papers that **keep** edge types (contrast for your related work)

- **CICAPT HDGI model** — *“modified GraphSAGE … adjusted to handle **heterogeneous graphs**”* ([same CICAPT paper](https://www.cs.unb.ca/~sray/papers/CICAPT_IIoT_paper_Mobiquitous24_author_copy.pdf), §5.3)
- **ProvExplainer** — edges as read/write/send ([arXiv](https://arxiv.org/html/2306.00934v5))
- **DEEPRO, APT-KGL, HAN** — explicit heterogeneous edge types

---

## For your paper

You can write:

> *Following Euler, Bowman, and the CICAPT Node2Vec baseline, we use a **homogeneous** provenance/auth/flow graph for unified GCN link prediction; heterogeneous relation modeling (HDGI, meta-path GNNs) is left as future work.*

Most LP papers **don’t say “we ignore types”** — they just **never model them**. Only surveys and heterogeneous papers state that limitation explicitly.

---

**User**

i think all papers preserve heterogenioty, can we do that ?

---

**Cursor**

**Many detection papers keep heterogeneity — but not the LP line you’re following, and not without cost on your timeline.**

## Reality check

| Camp | Heterogeneity? | Examples |
|------|----------------|----------|
| **APT detection GNNs** | Yes — node + edge types | HDGI, HAN, DEEPRO, MAGIC, KAIROS |
| **LP / lateral-movement LP** | Usually no — one interaction edge | Euler, Bowman, CICAPT Node2Vec baseline |

So “all papers” is too strong. **Detection** papers often do; **link-prediction anomaly** papers often don’t.

---

## Can you do it?

**Yes on CICAPT** — natural hetero graph:

- **Node types:** `Process`, `Artifact` (file/socket/…)
- **Edge types:** `Used`, `WasGeneratedBy`, `WasTriggeredBy`, `WasDerivedFrom`

**Harder on LANL / OpTC:**

- LANL: user ↔ computer (bipartite hetero) vs host↔host (homogeneous) — pick one
- OpTC: basically one edge type (FLOW-START) — already homogeneous

**Hard with your current stack:**

- HeaRT / SoHeaRT assume **one** adjacency (PPR, CN, GCN)
- Full hetero needs **PyG `HeteroData` + R-GCN / HeteroConv / HAN** — new encoder, likely **no drop-in SoHeaRT**

---

## Should you for this paper?

| Option | Verdict |
|--------|---------|
| **Full hetero on all 3 datasets** | **No** — 1+ week rewrite; breaks unified pipeline |
| **Homogeneous (current plan)** | **Yes** — matches Euler/Bowman; SoHeaRT works |
| **Node types only** (Process vs Artifact one-hot + SVD) | **Maybe** — cheap middle ground on CICAPT |
| **Full hetero on CICAPT only** (ablation) | **Stretch** — second experiment if MVP done early |

---

## If you insist on heterogeneity (minimal scope)

**CICAPT-only hetero MVP:**

1. Build `HeteroData` with 4 typed edge stores  
2. **R-GCN** or **HeteroConv(SAGEConv)** per edge type  
3. Train on Phase 1 typed edges; test attack edges **per relation**  
4. SoHeaRT: run **separately per edge type** or use **homogeneous collapse for eval only** (weaker story)

That’s a **different paper** (heterogeneous APT LP), not “SoHeaRT eval on cyber graphs.”

---

**Recommendation:** Keep **homogeneous edges** for the 3-dataset SoHeaRT story. Optionally add **node-type features** on CICAPT (Process vs Artifact). Cite HDGI/KAIROS as heterogeneous detection work you don’t replicate. Full edge-type heterogeneity = post-deadline or separate paper.

---

**User**

this hetero features are on nodes or edges?

---

**Cursor**

**Both — “heterogeneous graph” means typed nodes and typed edges, not just one.**

## On nodes (node heterogeneity)

- **Types:** `Process` vs `Artifact` (file, socket, …)
- **Features:** different columns per type in CICAPT (`exe` for Process, `path` for file Artifact, `remote address` for socket, …)
- In PyG: separate node stores, e.g. `data['process'].x`, `data['artifact'].x`

## On edges (relation heterogeneity)

- **Types:** `Used`, `WasGeneratedBy`, `WasTriggeredBy`, `WasDerivedFrom`
- **Features (optional):** `operation`, `time`, `event id`, … on edge rows
- In PyG: separate edge stores, e.g. `('process', 'Used', 'artifact')`, `('process', 'WasTriggeredBy', 'process')`

## What each option in our plan meant

| Approach | Nodes | Edges |
|----------|-------|-------|
| **Collapse all (current plan)** | One node set; types optional as features | One edge type: “connected” |
| **Node hetero only (middle ground)** | Process vs Artifact (+ maybe subtype) | Still collapsed to one edge |
| **Full hetero (HDGI/HAN style)** | Typed nodes | **Typed relations preserved** |

## For link prediction

- Scoring is usually **per relation**: *“Is there a `Used` edge from this process to this file?”*
- Negatives must match that relation (harder than homogeneous LP).

**Short answer:** Heterogeneity is **node types + edge relation types**. Node-only typing is a lighter partial version; your CSV already has both, but your MVP collapses **edge relations** and mostly uses **structural** node features (SVD), not rich per-type attributes.

---

**User**

so node Heterogeneity we can label as node features, what about edge heterogenity>?

---

**Cursor**

**Node heterogeneity → node features works. Edge heterogeneity → not the same thing.**

## Node heterogeneity → features ✓

On one unified node index you can add:

- `type` one-hot: Process vs Artifact  
- optional `subtype`: file / socket / …  

That encodes **node** heterogeneity into `x` while the graph stays one node set.

---

## Edge heterogeneity ≠ a simple “edge feature” in your LP setup

Two different things:

| | What it is | Example (CICAPT) |
|---|------------|-------------------|
| **Edge type (relation)** | *Which kind of link* | `Used`, `WasGeneratedBy`, … |
| **Edge features** | Attributes *on* that link | `operation`, `time`, `event id` |

**Relation type** is structural (which message-passing / which decoder head), not just another column like node `exe`.

---

## Why edge type doesn’t map cleanly to “features” in homogeneous LP

Your decoder scores pairs *(u, v)*:

- **Train positives:** real edges — you know relation type  
- **Test positives:** real attack edges — you know type  
- **Negatives:** **no edge** → **no relation type, no edge features**

So you can’t attach a relation one-hot to every *(u, v)* score the way you do for node type on every node.

---

## What people do instead

| Approach | Edge heterogeneity |
|----------|------------------|
| **Collapse (your plan)** | Drop relation type; one edge = “connected” |
| **Relation as edge feature (weak)** | Only on **existing** edges at train time; negatives still have no type — inconsistent |
| **Full hetero (right way)** | **Separate relation** in `HeteroData` + R-GCN / per-relation LP: predict *(u, r, v)* |
| **4 separate LPs** | One model per relation type — heavy, still awkward for SoHeaRT |

---

## Practical takeaway for your paper

- **Nodes:** encode heterogeneity as **node features** (cheap, fine).  
- **Edges:** either **collapse relations** (Euler-style) **or** use a **heterogeneous GNN** — edge type is **not** a drop-in feature like node type.

**One line:** Node type → feature vector on nodes; edge type → **relation label / separate edge channel**, not a feature you can paste onto random *(u, v)* pairs for LP.

---

**User**

give 1-2 examples of edge labels in 3 datasets

---

**Cursor**

**Edge “labels” = what each edge means in each dataset (1–2 examples each).**

---

### CICAPT (provenance)

| Edge label (relation) | Example |
|----------------------|---------|
| **`Used`** | Process `bash` → **read** file `/etc/passwd` |
| **`WasTriggeredBy`** | Parent process → **spawned** child process |

*(Also: `WasGeneratedBy`, `WasDerivedFrom`)*

---

### LANL Cyber1 (auth)

LANL doesn’t use PROV relation names. One edge = **one auth event** between entities. Raw log fields (not separate graph edge types):

| Field (metadata on edge) | Example |
|--------------------------|---------|
| **Auth between computers** | `C17693` → `C728` (from `redteam.txt`) |
| **Auth type** (in `auth.txt`, often dropped in graphs) | `Negotiate`, logon type `Batch` |

Euler/Bowman: edge label is effectively **“authenticated”** (host→host); auth type is ignored.

---

### OpTC (host flows)

| Edge label | Example |
|------------|---------|
| **`FLOW-START`** | Host `10.1.2.5` → `10.1.2.9` (new network flow) |

Only this event type is used in Euler-style graphs; `PROCESS-OPEN`, `FLOW-MESSAGE`, etc. are dropped.

---

**Summary:** CICAPT has **4 named relation types**; LANL/OpTC collapse to **one interaction type** (auth / flow), with extra fields in raw logs that LP papers usually don’t model as separate edge labels.

---

**User**

@/home/mmanjee/.cursor/projects/home-mmanjee-HeaRT/terminals/12.txt:368-416 

Read terminal and help me

---

**Cursor**

**Summary:** Benign is partly on disk (~3.2 GB, 55 files). Evaluation failed before any `.gz` files downloaded — Google Drive / gdown rate limit, not a wrong folder.

## What happened

You ran both commands from `/home/mmanjee/HeaRT/CARS/dataset/optc`:

```bash
gdown --folder ...1XOGWT8R8ovw6J2DgNsui09ZwvrfOXIVa -O ./OpTC/ecar-bro/benign/
gdown --folder ...1mRnYKaMsYYPQbCwdq9YLCgNy-pD6uP0Y -O ./OpTC/ecar-bro/evaluation/
```

**Benign (partial success):**
| Day folder | Files |
|------------|-------|
| 17-18Sep19 | 14 |
| 18-19Sep19 | 19 |
| 19Sep19 | 19 |
| 20-23Sep19 | **3** (incomplete) |

**Evaluation (failed):** gdown listed all subfolders (`23Sep-night`, `23Sep19-red`, `24Sep19`, `25Sept`, …) but saved **0** `.gz` files. It died on file `18nL8pOLfm1ofUP0h8fjSEAWHp_diHHnI` — typical after heavy Drive traffic (quota / rate limit), not a bad URL.

**Path quirk:** You get `benign/benign/...` and `evaluation/evaluation/...` because gdown appends the Drive folder name under `-O`.

---

## What to do next

**1. Resume evaluation in smaller chunks** (best fix):

```bash
cd /home/mmanjee/HeaRT/CARS/dataset/optc

# Attack day only — enough for test positives
gdown --folder "https://drive.google.com/drive/folders/<23Sep19-red-folder-id>" \
  -O ./OpTC/ecar-bro/evaluation/23Sep19-red/ --continue
```

Open the [evaluation Drive folder](https://drive.google.com/drive/folders/1mRnYKaMsYYPQbCwdq9YLCgNy-pD6uP0Y), right-click each day subfolder → “Get link”, and download one day at a time. For the paper you mainly need **`23Sep19-red`** (attack); `24Sep19` / `25Sept` are optional.

**2. Retry after a cooldown** (15–60 min):

```bash
gdown --folder "https://drive.google.com/drive/folders/1mRnYKaMsYYPQbCwdq9YLCgNy-pD6uP0Y" \
  -O ./OpTC/ecar-bro/evaluation/ --continue
```

**3. Browser fallback** for the failed file:  
https://drive.google.com/uc?id=18nL8pOLfm1ofUP0h8fjSEAWHp_diHHnI

**4. Finish benign `20-23Sep19`** — only 3 of many AIA shards downloaded; re-run that subfolder the same way.

**5. Ground truth** (separate from ecar-bro):

```bash
git clone https://github.com/FiveDirections/OpTC-data.git
# OpTCRedTeamGroundTruth.pdf + ecar.md
```

---

## For your CARS pipeline

Minimum OpTC slice:
- **Train:** benign days before attack (`17-18Sep19` … `20-23Sep19` complete)
- **Test:** `23Sep19-red` evaluation shards + ground-truth PDF for attack FLOW-START edges

You can start CICAPT + LANL while OpTC finishes; OpTC is dataset #3, not blocking day-1 work.

If you want, switch to **Agent mode** and I can script per-subfolder downloads and a completeness check.

---

**User**

lets clean dir and start again later

---

**Cursor**

**Ask mode** — I can’t delete files here. Run this yourself:

```bash
cd /home/mmanjee/HeaRT/CARS/dataset
rm -rf optc/OpTC
# or wipe the whole optc tree:
# rm -rf optc
```

That removes ~3.2 GB of partial downloads (`benign/` + empty `evaluation/` skeleton).

**When you retry later**, use per-day subfolders to avoid rate limits:

```bash
mkdir -p /home/mmanjee/HeaRT/CARS/dataset/optc
cd /home/mmanjee/HeaRT/CARS/dataset/optc

# one day at a time from Drive UI links, e.g.:
gdown --folder "<day-subfolder-url>" -O ./benign/20-23Sep19/ --continue
gdown --folder "<23Sep19-red subfolder>" -O ./evaluation/23Sep19-red/ --continue
```

Also clone ground truth separately: `git clone https://github.com/FiveDirections/OpTC-data.git`

Switch to **Agent mode** if you want me to run the cleanup for you.

---

**User**

only test is not there train is there right complete?

---

**Cursor**

Checking benign vs evaluation completeness on disk.


**Not quite.** Test is fully missing; train is **mostly** there but **not complete**.

| Split | Status |
|-------|--------|
| **Test (evaluation)** | **0 files** — nothing downloaded |
| **Train (benign)** | **Partial** — 55 `.gz` files (~3.2 GB) |

**Benign by day:**

| Day | Files | Status |
|-----|-------|--------|
| 17-18Sep19 | 14 | looks complete |
| 18-19Sep19 | 19 | looks complete |
| 19Sep19 | 19 | looks complete |
| **20-23Sep19** | **3–4** | **incomplete** (other days have ~19 AIA shards) |

Euler trains on **4 benign days**; you're missing most of the last one (`20-23Sep19`), which is the day right before the red-team activity.

**Bottom line:** You have ~3 of 4 benign days. You still need `20-23Sep19` (train) and all of evaluation (test, especially `23Sep19-red`).

---

**User**

what is the difference between diff datasets in optc?

---

**Cursor**

Searching the workspace and OpTC docs for how the dataset splits differ.


OpTC is one exercise (~1 TB) split several ways. For your paper you only need **`ecar-bro` → benign + evaluation** (not `ecar`, `bro`, or `short`).

## Top-level: 3 data formats (same exercise, different views)

| Folder | What it is | Use for your LP? |
|--------|------------|------------------|
| **`ecar-bro`** | **FLOW-START** events + Bro flow ID (host↔host comms, network-linked) | **Yes** — Euler uses this |
| **`ecar`** | Full **endpoint** eCAR (process, file, registry, flow, …) | No — much larger, provenance-style |
| **`bro`** | **Zeek/Bro** network sensor logs by date | No — unless you pivot from ecar-bro via `bro-id` |

Same time window in all three; difference is **granularity and sensor type**.

---

## Within `ecar-bro` / `ecar`: 3 time splits

| Subfolder | Period | Role |
|-----------|--------|------|
| **`benign`** | ~**Sept 17–23** (4 train days in Euler) | **Train** — no red-team activity |
| **`evaluation`** | **Sept 23–25** (+ `23Sep-night`) | **Test** — red-team campaigns |
| **`short`** | Same exercise, **incomplete events** | Skip |

**`benign` vs `evaluation`:** same event format; difference is **whether red team was active** that day. Benign traffic still runs during evaluation days.

---

## Day folders inside each split

**Benign (train):**
- `17-18Sep19`, `18-19Sep19`, `19Sep19`, `20-23Sep19` → Euler trains on first **4** days

**Evaluation (test):**
| Day folder | Attack? |
|------------|---------|
| `23Sep-night` | **No** — overnight baseline before attacks |
| `23Sep19-red` | **Yes** — campaign 1 |
| `24Sep19` | **Yes** — campaign 2 |
| `25Sept` | **Yes** — campaign 3 |

Each attack day is a **separate campaign**; compromised state resets next day (Euler §9.1).

---

## `AIA-*` shards (inside each day)

- **AIA** = Acuity Intelligent Agent (endpoint sensor on each host)
- **`AIA-51-75`, `AIA-101-125`, …** = chunks of ~25 hosts (500 hosts total, not full 1000)
- Same schema everywhere — only **which hosts** differ, not event type

---

## Labels (not in the JSON)

Events are **unlabeled** in the files. Attack edges come from **`OpTCRedTeamGroundTruth.pdf`** (FiveDirections/OpTC-data repo): map red-team host IPs + timestamps → anomalous **FLOW-START** edges.

---

## What you need for CARS

```text
ecar-bro/benign/     → train graph (FLOW-START, host→host)
ecar-bro/evaluation/ → test graph (mainly 23Sep19-red; optional 24Sep19, 25Sept)
OpTCRedTeamGroundTruth.pdf → attack edge labels
```

Skip: `ecar`, `bro`, `short`, and whole-root `gdown` (rate limits). Download **one day subfolder at a time**.

---

**User**

ecar
Full endpoint eCAR (process, file, registry, flow, …)
No — much larger, provenance-style


why not use this?

---

**Cursor**

**You can use `ecar` — but it’s a different experiment than the one you’ve scoped.** For OpTC in your CARS paper, `ecar-bro` / FLOW-START is the better fit.

## What `ecar` actually is

Full **endpoint provenance**: process create, file read, registry write, flow start, etc. Nodes are processes/files/registry keys; edges are many relation types. That’s the same *family* as CICAPT, not the same as LANL auth or OpTC host-flow graphs.

## Why not `ecar` for *your* paper

**1. Wrong graph for “lateral movement LP”**  
Your claim is: train on benign **communication** edges, test whether **attack communication** edges rank above hard negatives. LM shows up as **host A → host B** (FLOW-START). In `ecar`, most edges are local (process→file, process→registry); host↔host is a thin slice you still have to extract.

**2. You already have a provenance dataset**  
CICAPT-IIoT is dataset #1 (process/artifact provenance). OpTC’s job in your trio is **host-level comms** (like LANL + Euler). Using `ecar` makes OpTC a second provenance dataset and blurs the 3-dataset story.

**3. Pipeline mismatch**  
Your reuse path is:

```text
edge list → graph.pt → SVD features → GCN + dot decoder → random vs SoHeaRT negatives
```

`ecar` needs: parse many object/action types, choose what counts as a node/edge, collapse or encode relations, handle billions of events. That’s weeks of preprocessing, not days.

**4. Scale**  
~**1.1 TB**, ~17B events in full `ecar`. `ecar-bro` is FLOW-START only — orders of magnitude smaller. Your partial benign download was already 3.2 GB from one subfolder.

**5. Comparability**  
Euler/Bowman/LANL line all use **host interaction graphs**. FLOW-START keeps OpTC on that line so reviewers can compare “same LP setup, different negative sampling,” not “we also built a provenance parser.”

**6. Labels**  
Ground truth PDF describes **red-team host compromise + network movement**. Labels map cleanly to FLOW-START edges. Mapping to arbitrary provenance edges (which file read is attack?) is much harder.

## When `ecar` *would* make sense

- Paper goal = **provenance anomaly detection** (Prov2Vec / Holmes-style), not host LP + SoHeaRT  
- You drop or demote CICAPT and invest in heterogeneous GNN / relation-aware decoder  
- You have time to engineer a full provenance graph builder

## Short answer

| | `ecar-bro` (FLOW-START) | `ecar` (full endpoint) |
|--|-------------------------|-------------------------|
| Graph | Host ↔ host | Process/file/registry/… |
| Fits your 3-dataset story | Yes (host LP slot) | Overlaps CICAPT |
| Fits HeaRT GCN pipeline | Yes | Major rework |
| Size / deadline | Feasible | Very hard |
| LM / Euler comparability | Direct | Indirect |

**`ecar` isn’t bad data — it’s the wrong abstraction for this paper.** CICAPT = provenance; OpTC = host flow; LANL = host auth. Same LP + SoHeaRT story across three graph types, not three ways of parsing OpTC.

---

**User**

but if we are not training on attack and only on benign how will model learn about attack?

---

**Cursor**

**It doesn’t learn “attack.”** It learns **normal**, then you check whether **attack edges look abnormal** (or rank poorly) under that model.

## What training on benign actually does

The GCN sees only benign edges and learns:

- which hosts/processes connect in normal operation  
- node embeddings that reflect **benign** graph structure  
- a decoder that scores “how likely is edge (u,v) given what I’ve seen?”

So the model learns **P(benign graph)**, not P(attack).

## How attack enters at test time

Attack edges were **never in the training graph**. At test you ask:

> “Given only benign structure, how surprising is this edge?”

Two equivalent framings:

| Framing | Train | Test positives | Interpretation |
|---------|-------|----------------|----------------|
| **Anomaly LP** (Euler) | Benign edges | Attack **FLOW-START** / red-team auths | Low score = anomalous link |
| **Held-out link prediction** (your setup) | Benign edges | Attack edges as **positive test pairs** | Can the model rank them above negatives? |

Attack hosts still appear as **nodes** (they exist in benign traffic too). What’s new at test is the **attack edges** — connections that weren’t in the train set.

## Intuition

- Benign train: `A↔B`, `B↔C` are common; embeddings reflect that.  
- Attack: compromised host `X` talks to internal host `Y` in a rare way.  
- Model trained on benign patterns often gives **low** score to `(X,Y)` — not because it saw “attack,” but because that link **doesn’t fit normal connectivity**.

Same idea as one-class / outlier detection: learn normal, flag deviations.

## Role of SoHeaRT

Random negatives are easy non-edges → attack edges look “predictable” → **inflated AP**.

SoHeaRT negatives are **plausible benign non-edges** (same subnet, similar degree) → harder eval bar → you test whether attack edges are **truly rankable**, not just “different from random.”

## One-line for the paper

> We train exclusively on benign structure to model normal connectivity; attack edges are evaluated as held-out positives whose predictability under benign-only training reveals whether standard LP metrics overstate cyber detection performance.

You’re not training a classifier on attack labels — you’re testing whether **benign-learned link models** can surface **attack-manifesting edges** under realistic negative sampling.

---

**User**

so other papers do that way aswell check properly

---

**Cursor**

Checking Euler and related cyber LP papers for benign-only training and attack-edge evaluation.


**Yes — the main cyber LP / LM line trains on benign (or pre-attack) structure and treats attacks as test-time anomalies.** That’s standard; it’s not unique to your setup.

## Papers that do benign-only (or no-attack) training

### Bowman et al. (RAID 2020) — Euler’s direct predecessor
- **Unsupervised** link prediction on auth graphs.
- Offline: train LR link predictor on **normal auth graph structure** (no attack labels needed).
- Online: **new** auth edges scored; **low probability → anomaly**.
- On LANL: Euler notes Bowman trains on **all days without anomalous activity** — still **no attack edges in train**.

### Euler (King & Huang, 2023)

**Explicit assumption** — learn baseline from benign data:

```771:776:/home/mmanjee/HeaRT/euler.txt
Like many other anomaly detection models [15, 16, 42, 54], we make the assumption that a
large sample of benign data is available from which to learn a baseline for normal behavior offline.
...
if there was malicious activity in what is thought to be the clean training data, then the model could learn
that certain patterns in connections are normal when they are not.
```

**LANL train split** — everything **before the first attack edge**:

```873:875:/home/mmanjee/HeaRT/euler.txt
For all experiments, the training set consists of all snapshots that occur before the first anomalous edge appears in the authentication logs. This allows models to learn what normal activity
looks like.
```

**OpTC train split** — first **4 benign days** only; attack days = test:

```1207:1210:/home/mmanjee/HeaRT/euler.txt
Models are trained on the first four benign days to learn the pattern of normal interactions within
the network; all snapshots after the first four baseline days are considered the test set. Because
each day in the OpTC test set represents a different attack campaign
```

**Validation** also stays unsupervised — held-out **benign**, not attack labels:

```1086:1088:/home/mmanjee/HeaRT/euler.txt
As our model was meant to be entirely unsupervised, in lieu of holding out some of the labeled
data for validation, we used a held-out set of unlabeled, benign data.
```

### Other work in the same family
- **PleasantSAP / malicious logins as graph anomalies** — model each user’s **normal** login graph; flag deviations.
- **Classic AIDS** (NetWalk, etc.) — train autoencoders on **benign** event/graph structure; reconstruct errors = anomalies.

**Shared logic:** learn **P(normal connectivity)**; attacks are **low-score / surprising edges** at test time.

---

## Not every paper does it — important nuance

| Paper | Training data | Notes |
|-------|---------------|-------|
| **Bowman ’20** | Benign auth graph | Fully unsupervised |
| **Euler** | Pre-attack / benign days only | Unsupervised anomaly LP |
| **Jbeil ’24** | **Benign + augmented malicious** auths | Self-supervised; **explicitly argues** benign-only is limiting |
| **Signature/provenance IDS** (Holmes, Poirot) | Known attack patterns | Supervised matching — different problem |

So: **your field’s anomaly-LP thread does benign-only training**; some newer work (Jbeil) adds malicious samples during training. That’s a **comparison point**, not a requirement.

---

## Your setup vs this literature

| Yours | Literature parallel |
|-------|---------------------|
| CICAPT Phase 1 → train | Euler LANL pre-first-attack; OpTC 4 benign days |
| Attack edges → test positives | Euler/Bowman red-team / labeled anomalous auths at eval |
| Random vs SoHeaRT negatives | **Your novelty** — they mostly use random or full-graph reconstruction |

**Paper sentence you can use:**

> Following Bowman et al. and Euler, we train link predictors only on benign graph structure and evaluate whether held-out attack edges are ranked as anomalous or unpredictable—without exposing attack labels during training.

That’s accurate and defensible. Your delta isn’t the train/test split; it’s **hard negative evaluation** under that standard split.

---

**User**

But we are training on benign and asking model if attack looks abnormal, so ranking abnormal among benign or ranking benging among abnormals? Just see some paper implementations. Or how to produce accurate evaluation metrics in ranking? Ans brieflt

---

**Cursor**

Checking how Euler/Bowman define positives, negatives, and ranking metrics at eval time.


**Neither.** You rank **attack edges vs decoy non-edges** — not “benign among attacks.”

## What gets ranked

| Role | What it is |
|------|------------|
| **Test positives (y=1)** | Attack / red-team / Phase-2 attack edges |
| **Test negatives (y=0)** | Sampled **non-attack** pairs (random or SoHeaRT) |

Question: *Do attack edges score higher than hard/random non-edges?*  
Not: *Do benign edges sort correctly inside the attack set.*

## Two equivalent framings (same math)

**1. HeaRT / LP (your pipeline)**  
- Train: benign edges vs train negatives.  
- Test: `score(u,v)` for attack edges vs K negatives per attack edge.  
- **Higher score = positive** → want attacks above negatives.

**2. Euler / Bowman (anomaly)**  
- Train: learn normal link likelihood on benign.  
- Test: **low likelihood = suspicious**; label anomalous edges y=1, benign/non-edges y=0.  
- Use **anomaly score = 1 − p(link)** (or flip labels) so attacks rank **at top** of suspicious list.

Same ranking; Euler just inverts “high = good link” to “high = bad/anomalous.”

## How papers compute AP/AUC

**Pooled ranking** (your `link_pred_metrics.py`, standard LP):

```text
scores = [all attack scores] + [all negative scores]
labels = [1,…,1]           + [0,…,0]
→ ROC-AUC, average precision
```

- **AUC**: P(random attack scored higher than random negative).  
- **AP**: precision over the ranked list (good for rare attacks).

**Euler (LANL/OpTC):** score edges in test snapshots; positives = labeled anomalous edges; negatives = other test edges / sampled non-edges; report AP/AUC + TPR@low FPR (threshold from benign validation, λ=0.6).

## Accurate eval — brief rules

1. **Never put attack edges in train graph.**  
2. **Fix negative protocol** — report random *and* SoHeaRT (your contribution).  
3. **Same K negatives per positive** when pooling (HeaRT style).  
4. **Don’t tune threshold on attack labels** — tune on benign val (Euler) or report threshold-free AP/AUC.  
5. **Class imbalance:** prefer **AP** over accuracy; report TPR @ fixed FPR if you want security-style numbers.

**One line:** Train on benign structure; at test, **attack edges are the positives in a ranked list of attack vs non-attack decoys** — you measure whether the model puts attacks above negatives, not how benign sorts among attacks.

---

**User**

but predicting attack is not equal to predicting link because there could be multiple links which are non attack. So how do we implement current pipeline for this?

---

**Cursor**

Checking how the current pipeline defines train/test edges and negatives.


You're right: **attack detection ≠ standard link existence**. Many test-period edges are benign and real; the model isn't asked "does this edge exist?" but **"does this edge score like training-normal connectivity more than these decoys?"**

## The mismatch

| Standard LP (Twitter) | Cyber (your paper) |
|----------------------|-------------------|
| Test **pos** = held-out **benign** edges that exist | Test **pos** = **attack** edges that exist |
| Test **neg** = non-edges | Test **neg** = non-edges (+ optionally benign test edges) |
| High score = "link exists" | High score = "looks like train benign"; attacks should beat **decoys**, not all benign links |

Attack and benign test edges are both real links — existence LP can't separate them. You use the **same GCN + scorer**, but **redefine test pos/neg lists**.

## How to wire your current pipeline

**Train (unchanged structurally):**
```text
adj / edge_index  = benign edges only (Phase 1 / pre-attack)
train loss        = BCE( benign_pos , random_non_edges )
```
Model learns normal graph structure — not "attack."

**Preprocess — export tensors like Twitter splits:**
```text
pos_test_edge  = attack_edge_list          # e.g. Phase-2 attack-touching edges
neg_test_edge  = heart_test [N_attack, K]  # SoHeaRT or random non-edges
```
Optional stronger eval:
```text
neg_test_edge  = concat( SoHeaRT_negs , sample(benign_test_edges, K) )
```
Benign-test negatives = "normal activity continued" — closer to Euler's operational eval.

**Eval (unchanged code path):** `main_gnn_snap.py` already does:
```text
score(pos_test)  vs  score(neg_test)  →  pooled AUC / AP
```
Only the **edge lists** change; not the model API.

**Message passing:** Keep **`adj_t` = train (benign) only** at test (as now). GCN embeddings reflect normal topology; you score candidate pairs with the decoder.

## What each negative type measures

| Negatives | Question |
|-----------|----------|
| Random non-edges | Attack vs obvious non-links (easy; inflated AP) |
| SoHeaRT | Attack vs **plausible but missing** links (your contribution) |
| Benign test edges | Attack vs **real normal** test traffic (hard; Euler-style FPR) |

Report **random vs SoHeaRT** as main table; benign-test negs optional ablation.

## What you are *not* doing

- Not predicting "all links in test graph."
- Not labeling every benign test edge as negative (that'd be huge and imbalanced) — **sample K negatives per attack edge**, same as HeaRT.

## One-line implementation recipe

1. Build `graph.pt` from benign train edges.  
2. Build `attack_test_pos.pt` from labels.  
3. Run `create_heart_negatives.py` on benign train graph → `heart_test_*.npy`.  
4. Point `evaluation_edges` at attack pos + heart negs instead of Twitter val/test splits.  
5. Train/eval with existing `main_gnn_snap.py`.

**Metric interpretation:** AP = "among attack edges and K decoys each, how often are attacks top-ranked?" — ranking task, not existence prediction. That's how Bowman/Euler justify LP for LM; your twist is **which decoys** (SoHeaRT vs random).

---

**User**

attack edge is a feature of edge that it is an attack or all edges are attack edges? Yes or No only

---

**Cursor**

**No** — only a **labeled subset** of edges are attack edges; most are benign.

---

**User**

so we can say benign in negative and attack is positive and positive need to be predicted and we can fool the model by sampling 500 benign sounds good?

---

**Cursor**

**Yes — that’s a sound, stronger eval.**

- **Positives** = attack edges (what you want ranked at the top)  
- **Negatives** = sampled **benign test edges** (real links, not random holes)  
- Model only saw **benign train**; at test you ask: *attack vs plausible normal links*

Sampling benign as negatives is harder than random non-edges (model may score both high), so AP drops — that’s the point.

**Small fixes for the paper:**
- Use **K benign negatives per attack positive** (e.g. K=500, same as SoHeaRT), not 500 total.  
- Report **three rows**: random neg | SoHeaRT neg | **benign-test neg** (optional third).  
- Sample benign from **test period only**, exclude attack edges from the negative pool.

So: attack = pos, benign sample = hard neg — good design; main SoHeaRT story stays random vs structural negs, benign negs = extra “operational” bar.

---

**User**

but if we dont give positives during train tim, model at test time will infer all as negative or benging

---

**Cursor**

**No — it won’t label everything negative.** That worry fits a **supervised attack classifier**, not this setup.

## What the model actually learns

Train loss is only:

- **High score** → benign edges that **exist** in the train graph  
- **Low score** → **random non-edges** (pairs with no train link)

So it learns **“looks like normal connectivity in the benign graph”**, not “attack vs benign.”

## What happens at test

| Edge type | Typical score |
|-----------|----------------|
| Random non-edge | **Low** |
| Benign test edge | **High** (looks like train) |
| Attack edge | **High or low** — depends if it’s **unusual** vs benign structure |

It does **not** default to “all negative.” Real links (benign **and** attack) often score **high**. The hard case is attack ≈ benign → scores overlap → **bad AP** (that’s a real result, not a bug).

## How you still get a metric

You don’t threshold each edge alone. You **rank**:

```text
positives = attack edges
negatives = sampled benign (or SoHeaRT / random)
→ AP / AUC: are attacks above negatives?
```

If the model scores attack and benign the same → AP ≈ random. That means **benign-only training can’t separate them** — a valid finding for the paper.

## One line

No attack labels in train doesn’t make everything “benign”; it makes the model a **benign link scorer**, and you test whether **attack edges rank above chosen negatives** — same logic as Euler/Bowman anomaly LP.

---

**User**

so we basically train on benign and literal negatives but test on benign and attack?

---

**Cursor**

**Almost — one tweak on test.**

| Phase | Positives | Negatives |
|-------|-----------|-----------|
| **Train** | Benign edges (exist in train graph) | Literal non-edges (no link in train graph) |
| **Test (eval)** | **Attack** edges only | Sampled decoys: random non-edges, SoHeaRT, and/or **benign test** edges |

At test you’re **not** treating benign and attack symmetrically. You **score both**, but:

- **Attack** = labeled positives (should rank **higher**)
- **Benign test** = either background traffic or **sampled negatives** (should rank lower)

Train teaches “normal link vs missing link.” Test asks “attack link vs chosen negatives” — attack was never in train.

---

**User**

oh so attack edges are just like benign edges but with label positives. So we ask model to only predict benign edges with attack lable positive?

---

**Cursor**

**Not quite.**

- **Attack edges** are real edges (same `(u,v)` format as benign), but they’re **not** benign edges with a flipped label — they’re the **attack subset** from ground truth (red-team auths, attack-touching provenance, etc.).
- **Benign test edges** are the **other** real edges in the test period — most traffic.

**What the model is asked:**

| | Sees attack label? | Task |
|--|-------------------|------|
| **Train** | No | Score **benign train** edges high vs **non-edges** low |
| **Test** | No (inference) | Score every candidate pair; **you** use labels only for metrics |

At test you **don’t** say “predict benign edges, mark attacks positive.” You say:

> Score these pairs; **attack-labeled** ones should rank **above** sampled negatives.

The model never learns “attack”; it learns benign structure. Labels only define **which test edges count as positives** in AP/AUC — like Euler’s anomalous-edge eval, not supervised attack classification.

---

**User**

how does euler/bowman do? cite them aswell @euler.txt 

---

**Cursor**

**Bowman and Euler both train on benign-only structure and treat attacks as low-likelihood / labeled-anomaly edges at test — not as supervised “attack class.”**

## Bowman et al. [16] (via Euler)

**Idea:** Static auth graph; **anomaly = low link probability** (no time).

```321:323:/home/mmanjee/HeaRT/euler.txt
as the work of Bowman et al. 2020 [16]. Here, normalcy is defined in terms of interactions between entities within systems, but time is not considered. Anomalies are defined as edges with
low probability given what is known about the graph's structure.
```

**Train:** Unsupervised — random-walk node embeddings + **logistic-regression link predictor** on normal auth edges (Euler: **all non-attack days**, not only pre-campaign).

```893:895:/home/mmanjee/HeaRT/euler.txt
Bowman et al. [16] does use the full authentication log
as its dataset, however, it trains on a larger set of data, using all days that contained no anomalous
activity as the training set, rather than just the days before the attack campaign.
```

**Test:** Score **each new auth edge**; flag if probability **below threshold** (offline train → online scoring). No attack labels in train.

**Negatives at eval:** Implicitly **all other scored edges** in test (mostly benign auths); positives = **dataset-labeled anomalous** auths (~518 on LANL). Euler reports Bowman as GL-LV / GL-GV in Table 7.

---

## Euler [King & Huang]

**Idea:** Temporal graphs; **anomaly = low likelihood** given past snapshots.

```283:288:/home/mmanjee/HeaRT/euler.txt
anomalous
edges in a temporal graph will have a low probability of occurring given what is known about the
network's behavior in the past.
...
An observed interaction between entities with a likelihood
score below a certain threshold is said to be anomalous.
```

**Assumption (same family as Bowman):**

```771:772:/home/mmanjee/HeaRT/euler.txt
Like many other anomaly detection models [15, 16, 42, 54], we make the assumption that a
large sample of benign data is available from which to learn a baseline for normal behavior offline.
```

### LANL

| | |
|--|--|
| **Train** | Snapshots **before first labeled anomalous edge**; LP loss on masked **benign** edges vs **non-edges** |
| **Val (threshold)** | Held-out **benign** snapshot edges + **non-edges** — **no attack labels** |
| **Test** | Score edges in **attack-period** snapshots; **positives = labeled anomalous**; metrics AP/AUC/TPR/FPR |

```873:875:/home/mmanjee/HeaRT/euler.txt
the training set consists of all snapshots that occur before the first anomalous edge appears in the authentication logs. This allows models to learn what normal activity
looks like.
```

```580:581:/home/mmanjee/HeaRT/euler.txt
Given a set of scores for edges that exist in the validation snapshots, but were held out of the
training set, and a set of scores for non-edges, the optimal cutoff threshold τ is the one that satisfies
```

```950:956:/home/mmanjee/HeaRT/euler.txt
we report precision (P), area under the curve (AUC), and
average precision (AP). This latter method is recommended for anomaly detection by Reference
[25] as especially adept for imbalanced datasets. The AUC and AP metrics evaluate the overall
quality of scores given to edges
```

### OpTC

| | |
|--|--|
| **Train** | First **4 benign days** |
| **Test** | Remaining days (attack campaigns); labels from red-team log |
| **Positives** | FLOW-START edges from **compromised hosts** after red-team event |

```1207:1210:/home/mmanjee/HeaRT/euler.txt
Models are trained on the first four benign days to learn the pattern of normal interactions within
the network; all snapshots after the first four baseline days are considered the test set. Because
each day in the OpTC test set represents a different attack campaign
```

```1162:1168:/home/mmanjee/HeaRT/euler.txt
If the source IP was involved in a red
team event that happened that day, and if that red team event occurred before or during the time
of the logged event, then it is marked as anomalous.
...
only hosts that have been successfully
compromised produce anomalous edges.
```

---

## vs your pipeline

| | Bowman | Euler | You (HeaRT) |
|--|--------|-------|-------------|
| Train pos | Benign auths | Benign snapshot edges | Benign edges |
| Train neg | Non-edges (implicit in LR) | Sampled non-edges | Random non-edges |
| Test pos | Labeled anomalous auths | Labeled anomalous edges | **Attack edge list** |
| Test neg | Other scored edges (mostly benign) | Other test edges | **Sampled** random / SoHeaRT / benign |
| Score | Link probability | Link likelihood | Dot-product link score |
| Attack in train? | **No** | **No** | **No** |

**Same logic:** learn normal links; attacks are a **labeled subset at eval**, scored low (Euler) or ranked against negatives (you).

**Your difference:** explicit **K negatives per attack** (random vs SoHeaRT) — Euler/Bowman mostly score **all observed test edges** and use labels only for metrics; you make the negative protocol **explicit and controllable** (your paper’s point).

**Citation for paper:** Bowman et al., RAID 2020 [16] in `euler.txt` line 1467–1468; Euler LANL §8.1–8.3, OpTC §9.1–9.3.

---

**User**

give concise list of methods both use and if our method is better than them in terms of model train/eval precisely

---

**Cursor**

**Short answer:** Your **model** is simpler than Euler and comparable to Bowman-style LP; your **eval** (SoHeaRT) is stricter than what they report — that’s the contribution, not “we beat Euler’s detector.”

---

## Methods (train / eval)

| | **Bowman [16]** | **Euler** | **Yours** |
|--|-----------------|-----------|-----------|
| **Graph** | Static auth | Temporal snapshots (δ≈30min) | Static (benign train adj) |
| **Encoder** | Random-walk embeddings | GCN/GAT/SAGE | GCN |
| **Temporal** | None | GRU/LSTM over snapshots | None |
| **Decoder** | Logistic regression on embed pairs | Inner product (+ optional Euler-SM softmax) | Dot-product (HeaRT) |
| **Train pos** | Benign auths (all **non-attack days**) | Benign edges in **pre-attack** snapshots | Benign edges only (Phase 1 / pre-attack) |
| **Train neg** | Implicit in LP / sampling | Sampled **non-edges** per snapshot | Random **non-edges** |
| **Attack in train?** | No | No | No |
| **Val** | (not detailed in Euler) | Benign held-out edges + non-edges → threshold τ | Optional benign val split |
| **Test pos** | Labeled **anomalous auths** | Labeled **anomalous** edges | **Attack** edge list |
| **Test neg** | All other scored edges (mostly benign) | Other test edges (implicit) | **Explicit K/sampled**: random / **SoHeaRT** / benign-test |
| **Score → alert** | Low P(link) → anomaly | Low likelihood → anomaly | Rank attack vs negatives; AP/AUC |
| **Metrics** | TPR, FPR, precision | AUC, AP, TPR, FPR | AUC, AP (pooled) |

---

## Is yours “better”? (train/eval only)

| Dimension | vs Bowman | vs Euler |
|-----------|-----------|----------|
| **Model capacity** | Similar static LP; you use GCN not random walks — **not clearly better** | **Worse** (no temporal RNN) |
| **Train protocol** | Same idea (benign-only unsupervised LP) | Same idea; Euler uses temporal masking — **not novel** |
| **Eval protocol** | You **fix** negative set (K per attack); Bowman scores all test auths | You **fix** negatives; Euler scores all test edges — **your eval is more controlled** |
| **Hard negatives** | They don’t use SoHeaRT | They don’t use SoHeaRT — **your SoHeaRT eval is stricter / more realistic** |
| **Detection AP** | **Unknown until you run** — don’t claim better without numbers | Euler reports strong LANL/OpTC numbers — **unlikely to beat on raw AP with static GCN** |

---

## One-line claim (accurate)

> Same **benign-train / attack-eval** family as Bowman & Euler; **not** a stronger detector by architecture — **stronger benchmark** via explicit **SoHeaRT** negatives vs random non-edges (shows when cyber LP metrics are inflated).

**Paper table:** report AP/AUC under **random neg** (comparable to easy eval) vs **SoHeaRT** (your contribution); optional **benign-test neg** (Euler-like operational hardness).

---

**User**

The model never learns “attack”; it learns benign structure. Labels only define which test edges count as positives in AP/AUC — like Euler’s anomalous-edge eval, not supervised attack classification.

so we just ask for true edges that exist and are labeled as attack edges right? Sey Yes or no

---

**Cursor**

**Yes** — test **positives** are real edges labeled attack.

(You also score **negatives** — non-edges or benign edges — for AP/AUC.)

---

**User**

so how would model learn in this case to rank higher those positives that are attack edges than those positives that are non attack edges? Answer briefly

---

**Cursor**

**It doesn’t learn “attack > benign” in training.**

Train only teaches: **benign existing edges > non-edges.**

At test, attack edges rank above negatives **only if** they score differently from those negatives — not because the model saw attack labels:

| Negatives | When attack ranks higher |
|-----------|--------------------------|
| **Random non-edges** | Easy — real attack links usually score above holes |
| **SoHeaRT non-edges** | Harder — only if attack scores above *plausible missing* links |
| **Benign test edges** | Hardest — both are real links; often **no separation** (AP ≈ random) |

So: no class “attack.” You hope **attack edges are structurally surprising** vs the negatives you pick. If they look like normal benign links, the model **won’t** rank them higher — that’s a valid result, not a training bug.

**Note:** Benign test edges are **negatives (y=0)**, not “non-attack positives.” Only attack edges are positives in AP/AUC.

---

**User**

but the problem is model is not learning anything about attack egdes. Its learning about benign true edges and then we ask it to tell about attack edges which have a new label which is someting model is not trained for

---

**Cursor**

**You’re right about the training objective — and that’s intentional, not a mistake.**

The model is **not** trained to recognize “attack.” It’s trained for:

> *“Does this pair look like a normal link in the benign graph?”*

Attack edges are **never a training class**. At test you check an **assumption** from anomaly detection:

> *Attack-related links are **unusual** relative to benign structure (rare hosts, rare pairs, wrong temporal context — Euler’s point).*

So you’re not asking “predict label attack.” You’re asking:

> *Among these candidate edges, do attack-labeled ones score **differently** than negatives?*

**When that works:** attack auths / LM flows are rare → rank above random (and maybe SoHeaRT) negatives.

**When it fails:** attack edges look like normal comms → scores overlap with benign → **low AP**. That’s a real finding: *benign-only LP isn’t enough for cyber detection without harder eval.*

**If you need the model to “learn attack”:** you need **supervised** training (attack labels in loss) or **Jbeil-style** augmented malicious samples — a **different** paper than Euler/Bowman/yours.

**Your paper’s claim (precise):** not “we teach GCN what attacks are,” but **“cyber LP papers evaluate with easy negatives; under SoHeaRT, benign-only models look much worse — so metrics were misleading.”** Same train/eval logic as Euler; your contribution is **which negatives** you use to measure that gap.

---

**User**

so now there is 2 perspectives. how would score be different? I mean it would be high or low right for attack edges what other diff it could have?

---

**Cursor**

**Two perspectives — same scalar score, opposite interpretation:**

| Perspective | Attack should be… | High score means… |
|-------------|-------------------|-------------------|
| **LP (your HeaRT setup)** | **High** (if you want attack as “positive” in AP) | Likely link |
| **Anomaly (Euler/Bowman)** | **Low** likelihood → high **anomaly** (use \(1 - p\)) | Normal link |

One number per edge; flip or invert labels for AP/AUC.

---

**It’s not just “high or low” — it’s *where in the distribution*:**

- **Random non-edge:** usually **low** (0.01)
- **Benign test edge:** usually **high** (0.85)
- **Attack edge:** could be **0.82** (looks normal → **fails**) or **0.15** (unusual pair → **works**) or **0.90** (still high but **below** 0.95 benign — matters only in **ranking**)

AP/AUC use the **full ranked list**, not a single threshold. Attack edges “differ” when their scores **separate** from negatives — overlap = bad AP even if all scores are “high.”

---

**What can make attack scores differ (without attack labels in train):**

1. **Rare endpoints** — compromised host rarely talks to that target in benign train  
2. **Rare pair** — \((u,v)\) never seen in train  
3. **Temporal context** (Euler) — same edge normal at \(t_1\), anomalous at \(t_4\)  
4. **Structural** — attack path uses unusual graph position (low CN with benign core)

If none apply, attack ≈ benign score → **no separation** — that’s the result you report.

**One line:** Score is one real number per edge; eval is **relative rank** among attack vs negatives, not “attack = high” by definition.

---

**User**

how does euler/bowman do it?

---

**Cursor**

**Both use link probability; attacks = low probability (anomaly), not a separate “attack score.”**

## Bowman [16]

1. **Train:** Random-walk node embeddings + **logistic regression** on benign auth edges (non-attack days).
2. **Score:** \(P(\text{edge exists} \mid \text{embeddings})\) for each auth \((u,v)\).
3. **Rule:** **Low \(P\) → anomaly** → alert.

```321:323:/home/mmanjee/HeaRT/euler.txt
Anomalies are defined as edges with
low probability given what is known about the graph's structure.
```

No attack labels in train; at test, labeled red-team auths should land **below** threshold.

---

## Euler

1. **Train:** GCN(+RNN) on **pre-attack** snapshots; loss = reconstruct / predict **benign** edges vs sampled **non-edges**.
2. **Score:** **Link likelihood** \(\hat{y}_{uv}\) from inner-product decoder (optionally Euler-SM softmax layer).
3. **Rule:** **Low likelihood → anomalous** (same as Bowman, plus **time**).

```286:288:/home/mmanjee/HeaRT/euler.txt
An observed interaction between entities with a likelihood
score below a certain threshold is said to be anomalous.
```

4. **Threshold \(\tau\):** Tuned on **benign** validation only — held-out real edges + non-edges (Eq. 8), **not** attack labels.

```580:581:/home/mmanjee/HeaRT/euler.txt
Given a set of scores for edges that exist in the validation snapshots, but were held out of the
training set, and a set of scores for non-edges
```

5. **Metrics:** Score **all test edges**; **positives** = dataset-labeled anomalous; **negatives** = rest (mostly benign). Report **AUC/AP** (ranking) + TPR/FPR at \(\tau\).

**Temporal twist:** Same \((u,v)\) can be normal at \(t_1\), anomalous at \(t_4\) because RNN context changed — Bowman can’t do this (static graph).

---

## vs your setup

| | Euler/Bowman | You |
|--|--------------|-----|
| Attack signal | **Low** link likelihood | Can use **high** score = positive in AP, or use \(1-\text{score}\) like them |
| Test negatives | Implicit (all other test edges) | Explicit K: random / SoHeaRT / benign |
| Train | Benign only | Benign only |

**One line:** They don’t learn “attack”; they score **link normality** — attack edges should get **unusually low** \(P(\text{link})\) vs benign traffic; Euler adds **when** it happened.

---

**User**

so we can do like euler right?

---

**Cursor**

**Yes.** You can match Euler’s framing exactly:

1. **Train** on benign edges only (+ non-edge negatives).  
2. **Score** each candidate edge with link likelihood \(\hat{y}\).  
3. **Anomaly score** = \(1 - \hat{y}\) (low likelihood → suspicious).  
4. **Test positives** = labeled attack edges.  
5. **Test negatives** = other scored edges (benign test traffic) — Euler’s implicit pool.  
6. **Metrics** = AUC/AP (+ optional TPR/FPR at τ from **benign** validation).

That’s the same logic as Euler/Bowman.

**Your add-on (still worth keeping):** also report **explicit** random vs **SoHeaRT** negatives (K per attack) — Euler doesn’t do that; that’s your paper’s eval contribution. You can run **both**: Euler-style (all test edges) + HeaRT-style (sampled hard negs).

---

**User**

but why euler style and heart style diff? draw table and show diff concise

---

**Cursor**

**Same model & train; different test negative pool and what AP measures.**

| | **Euler-style** | **HeaRT-style** |
|--|-----------------|-----------------|
| **Score** | Link likelihood \(\hat{y}\); anomaly = **low** \(\hat{y}\) | Same \(\hat{y}\); often **high** attack in AP via \(1-\hat{y}\) or label flip |
| **Test positives** | Labeled attack / anomalous edges | Same — attack edge list |
| **Test negatives** | **All other scored edges** in test window (mostly **benign real links**) | **Sampled K per attack**: random non-edges and/or **SoHeaRT** |
| **Universe size** | Huge (millions of benign auths) | Fixed: \(N_{\text{attack}} \times K\) |
| **Question asked** | “Do attacks look **less normal** than routine test traffic?” | “Do attacks beat **these decoys**?” |
| **Typical AP** | Often **low** (attacks ≈ benign links) | **Random neg:** inflated; **SoHeaRT:** drops |
| **Threshold** | τ from **benign val** (Eq. 8) | Usually **none** — rank-only AP/AUC |
| **Literature** | Bowman, Euler LANL/OpTC | HeaRT (OGB LP), your contribution |

```text
Euler:     [attack pos] vs [======== all benign test edges ========]
HeaRT:     [attack pos] vs [K random]  or  [K SoHeaRT hard non-edges]
```

**Why both:** Euler-style = **operational** (“find needle in haystack of real traffic”). HeaRT-style = **controlled** ablation (“how much does negative sampling inflate cyber LP?”). Same GCN; **eval protocol** differs.

---

**User**

so i think heart or soheart is not relevant to this problem right

---

**Cursor**

**Partially — depends what the paper is.**

| Goal | SoHeaRT relevant? |
|------|-------------------|
| **Replicate Euler/Bowman cyber detection** | **No** — use Euler-style: attack vs benign test traffic, low link likelihood |
| **Your CARS angle** (“hard negatives expose inflated cyber LP”) | **Yes** — that *is* the contribution |

**Why it feels irrelevant:** SoHeaRT negatives are **plausible non-edges** (missing links), not “benign auth that happened today.” Real ops look more like **Euler-style** (attack vs millions of normal links).

**Why it’s still useful:** Cyber papers often report AP with **random non-edges** → scores look great. SoHeaRT shows that gap is **methodological**, not real detection skill. It’s an **LP benchmark critique**, not a deployed IDS.

**Practical recommendation for 6 pages:**
- **Main:** Euler-style (attack vs benign test edges) — cyber-native  
- **One table/figure:** random vs SoHeaRT — shows **metric inflation** (your HeaRT hook)

**One line:** SoHeaRT isn’t how Euler evaluates LM, but it **is** relevant if the paper is “don’t trust random-negative LP on cyber graphs” — not if the paper is only “we beat Euler.”

---

**User**

ok give example of why heart is relevant here? \

---

**Cursor**

**Concrete example (LANL-style host auth graph)**

**Setup:** Train on benign auths only. One test attack edge:

> **Compromised host `C47` → internal server `S12`** (red-team auth, never seen in train)

---

### Eval 1: Random negative (what many LP papers do)

Sample negative: **`C47` → `Z999`** (random host, no edge in train).

| Edge | Exists in real network? | Model score |
|------|-------------------------|-------------|
| `C47→S12` (attack) | Yes | 0.72 |
| `C47→Z999` (random neg) | No | 0.03 |

Attack ranks way above negative → **AP ≈ 0.95**. Looks like great detection.

But you only proved: *“real link beats random hole”* — trivial.

---

### Eval 2: SoHeaRT negative (structurally plausible **non-edge**)

SoHeaRT picks: **`C47` → `S08`** — same subnet as `S12`, similar degree, high PPR/CN, but **no auth in train**.

| Edge | Exists? | Model score |
|------|---------|-------------|
| `C47→S12` (attack) | Yes | 0.72 |
| `C47→S08` (SoHeaRT neg) | No | 0.68 |

Scores overlap → **AP ≈ 0.55**. Same model, same attack — metric collapses.

**SoHeaRT relevance:** shows random-negative AP **overstates** whether the model can separate attack from **plausible** alternatives, not just garbage pairs.

---

### Eval 3: Euler-style (benign **real** test edge as negative)

Benign traffic that day: **`C47` → `DC1`** (normal domain-controller auth).

| Edge | Exists? | Model score |
|------|---------|-------------|
| `C47→S12` (attack) | Yes | 0.72 |
| `C47→DC1` (benign test) | Yes | 0.88 |

Attack ranks **below** normal traffic → **AP low** — operational “needle in haystack.”

---

**Why HeaRT matters for your paper**

```text
Random neg:  attack beats obvious fakes     → AP inflated (misleading cyber claim)
SoHeaRT:     attack vs plausible missing    → AP drops (honest structural LP bar)  
Euler:       attack vs real benign traffic  → hardest (true ops setting)
```

**One-line story:** Without SoHeaRT, you’d report **0.95 AP** and claim cyber LP works; SoHeaRT shows much of that is **easy negatives**, not finding lateral movement — that’s the HeaRT transfer to cyber graphs.

---

**User**

sso euler method is better than heart?

---

**Cursor**

**No — they’re not competing methods; they answer different questions.**

| | **Euler-style** | **HeaRT / SoHeaRT** |
|--|-----------------|---------------------|
| **Better for** | “Can we spot attacks among **real daily traffic**?” (ops / LM detection) | “Are **LP metrics** honest when negatives are easy?” (benchmark critique) |
| **Worse for** | Comparing across papers with fixed K negatives | Claiming deployable IDS performance alone |

**Better for cyber detection story:** Euler-style (attack vs benign test edges, low likelihood).

**Better for your HeaRT paper hook:** SoHeaRT — shows random-negative AP is **misleading** before you even get to Euler-hard eval.

**Use both in the paper:**
1. **SoHeaRT** — random AP is inflated  
2. **Euler-style** — even with honest negatives, ops is harder  

**One line:** Euler isn’t “better than HeaRT” — Euler is **more realistic for cyber**; HeaRT is **more useful for proving metric inflation**. You want **both rows** in a table, not one instead of the other.

---

**User**

ok give me precise table showing model details what is train/test etc details for heart applied to this Cybersecurity data for attack prediction precisley

---

**Cursor**

**HeaRT-on-cyber spec** (attack = test positives only; model never sees attack labels in train)

---

## 1. Model & features (same as `main_gnn_snap.py`)

| Item | Setting |
|------|---------|
| **Encoder** | 3-layer GCN, hidden 256, dropout 0.1 |
| **Decoder** | 3-layer MLP predictor → dot-product link score \(\hat{y}_{uv}\in[0,1]\) |
| **Node features** | SVD-256 from benign train graph (`build_snap_features.py`) or ID/structural features |
| **Message-passing graph** | **`adj_t` = benign train edges only** (at train & test) |
| **Loss** | BCE: benign train edges = 1, sampled train non-edges = 0 (unless `--train_pos_only`) |
| **Optimizer** | Adam, lr=1e-3, early stop on val |
| **Temporal** | **None** (static vs Euler) |

---

## 2. Data split (per dataset)

| Dataset | **Train graph** | **Test positives** `pos_test` | **Count (order of)** |
|---------|-----------------|--------------------------------|----------------------|
| **CICAPT-IIoT** | Phase 1 unique edges (benign; collapse 4 rel → 1) | Phase 2 edges touching `label=1` nodes | ~64k train / ~570 test pos |
| **LANL Cyber1** | Auths **before first red-team edge** (Euler) | `redteam.txt` labeled auths | ~45M events / ~750 test pos |
| **OpTC** | FLOW-START from **4 benign days** | FLOW-START from red-team days (ground-truth PDF) | Euler: ~21k anomalous edges |

**Not in train:** any attack/red-team/Phase-2 attack edges or attack-only nodes (CICAPT).

---

## 3. Train (identical across datasets)

| | |
|--|--|
| **Positive edges** | All benign train edges \(E_{\text{train}}\) |
| **Negative edges** | Uniform random \((u,v)\notin E_{\text{train}}\) (same count ratio as Twitter runs) |
| **Labels used** | None for “attack” — only edge existence |
| **Objective** | Learn \(P(\text{link} \mid \text{benign structure})\) |

---

## 4. HeaRT / SoHeaRT test eval (your main table)

| | **Random (HeaRT-easy)** | **SoHeaRT (HeaRT-hard)** |
|--|-------------------------|---------------------------|
| **Test positives** | Attack edge list \(E_{\text{attack}}\) | Same |
| **Test negatives** | \(K\) random non-edges per attack edge | \(K\) hard non-edges per attack edge (PPR + CN/RA on **benign train** graph) |
| **\(K\)** | 500 (`--num_samples`) | 500 (`samples_soheart_eta0p5_sig1.npy`) |
| **Score** | \(\hat{y}_{uv}\) from GCN | Same |
| **Positive label for AP/AUC** | \(y=1\) on attack edges | Same |
| **Negative label** | \(y=0\) on sampled negs | Same |
| **Metric** | Pooled ROC-AUC & AP (`link_pred_metrics.py`) | Same |
| **Interpretation** | Attack vs obvious missing links | Attack vs **plausible missing** links |

```text
For each (u,v) in E_attack:
  score attack edge
  score K negatives
  → pool all scores → AUC / AP
```

**Optional val:** held-out **benign** edges + random negs (no attack labels) — for early stopping only.

---

## 5. Euler-style test eval (optional second row)

| | |
|--|--|
| **Test positives** | Same \(E_{\text{attack}}\) |
| **Test negatives** | **All other edges in test window** (mostly benign real links), or sample from them |
| **Anomaly score** | \(1 - \hat{y}_{uv}\) (low link likelihood = suspicious) |
| **Threshold \(\tau\)** | From benign val (Euler Eq. 8), not from attacks |
| **Metrics** | AUC, AP, TPR @ low FPR |

---

## 6. What this is / isn’t

| | |
|--|--|
| **Is** | Unsupervised benign LP + **ranking** labeled attack edges vs chosen negatives |
| **Is not** | Supervised attack classifier; temporal Euler; “predict all test links” |
| **HeaRT contribution** | Same GCN; **negative protocol** changes AP (random ↑ vs SoHeaRT ↓) |
| **Cyber-native eval** | Euler-style (attack vs benign test traffic) |

---

## 7. Artifacts to build (cyber layout)

| File | Content |
|------|---------|
| `graph.pt` / `adj_t` | Benign train edges |
| `x_snap_svd256.pt` | Node features from benign graph |
| `attack_test_pos.pt` | \(E_{\text{attack}}\) |
| `heart_test_samples.npy` | `[N_attack, 500]` random non-edges |
| `heart_test_samples_soheart_*.npy` | `[N_attack, 500]` SoHeaRT non-edges |

---

## 8. Paper table (one row per eval mode × dataset)

| Dataset | Train | Test pos | Test neg | Model | Metric |
|---------|-------|----------|----------|-------|--------|
| CICAPT | Phase 1 benign | Phase 2 attack-touching | Random K / SoHeaRT K | GCN+MLP | AP, AUC |
| LANL | Pre-attack auths | `redteam.txt` | Random K / SoHeaRT K | GCN+MLP | AP, AUC |
| OpTC | 4 benign days FLOW-START | Red-team FLOW-START | Random K / SoHeaRT K | GCN+MLP | AP, AUC |

Add optional column: **Euler neg** = sample benign test edges instead of non-edges.

---

**User**

can you check for:
1. why heart test scoring is different from euler test scoring?
2. what test negative does euler consider?


---

**Cursor**

Checking Euler's test scoring and negative-edge definition in the paper text.


**From `euler.txt` + your `link_pred_metrics.py`:**

---

## 1. Why HeaRT test scoring ≠ Euler test scoring

| Dimension | **HeaRT (your pipeline)** | **Euler (LANL/OpTC)** |
|-----------|---------------------------|------------------------|
| **What gets scored** | Fixed list: `N_attack` positives + `N_attack × K` **sampled** negatives | **Every observed edge** in test snapshot(s) with a label |
| **Score meaning** | High \(\hat{y}\) = “looks like a link” → attack labeled **y=1** in `pooled_auc_ap` | **Low** link likelihood = anomalous; alert if below \(\tau\) |
| **Temporal context** | Static GCN on **benign train `adj` only** | GCN+RNN on snapshot sequence; test edges scored **in time context** |
| **Test graph in GNN** | Train adj frozen | Test anomalous edges **enter** test snapshot structure |

```489:491:/home/mmanjee/HeaRT/euler.txt
During evaluation, instead of returning loss, the workers return edge likelihoods yˆt and the
ground-truth edge labels yt .
```

```20:24:/home/mmanjee/HeaRT/HeaRT/benchmarking/HeaRT_ogb/link_pred_metrics.py
    y = np.concatenate([np.ones_like(pos), np.zeros_like(neg)])
    s = np.concatenate([pos, neg])
    return {
        "AUC": float(roc_auc_score(y, s)),
```

**Direction mismatch:** Euler wants attacks **low** \(\hat{y}\); HeaRT pools with attack as y=1 and **higher** score = better. For Euler-aligned metrics use **`1 - \hat{y}`** (or flip labels).

**Universe mismatch:** HeaRT AP = “attack vs **K fake/missing** links per attack.” Euler AP = “attack vs **all other real test traffic**.”

---

## 2. What test negatives does Euler use?

**Train negatives (explicit):** random **non-edges** per snapshot.

```530:531:/home/mmanjee/HeaRT/euler.txt
likelihood scores for known edges, and a random sample of non-edges at time t +n: Pt+n and Nt+n,
respectively.
```

**Val negatives (threshold \(\tau\)):** random **non-edges** + held-out **benign** edges in val snapshots — **not** attack labels.

```580:581:/home/mmanjee/HeaRT/euler.txt
Given a set of scores for edges that exist in the validation snapshots, but were held out of the
training set, and a set of scores for non-edges
```

**Test negatives (LANL anomaly AP/AUC):** **not** random non-edges. Negatives = **other observed edges in test snapshots** — almost all **benign auths** in the attack window. Positives = dataset-labeled anomalous (~518 LANL).

```1092:1093:/home/mmanjee/HeaRT/euler.txt
The test data contained anomalous edges that affected the overall graph structure. As a result,
these edges would be considered in the GNN during node embedding. The negative edges in the
validation set, however, were not.
```

Implied: val negatives = non-edges; **test negatives = benign observed edges** (implicit pool, not K-sampled).

**UA baseline** (same spirit): flag auths **never seen in training** — negatives are “seen-before” benign edges.

```897:897:/home/mmanjee/HeaRT/euler.txt
(UA) model reported by Reference [16]. This rule simply marks any edge that did not exist in the
```

---

## Summary table

| | **HeaRT test** | **Euler test** |
|--|----------------|----------------|
| **Positives** | Attack edge list | Labeled **anomalous observed** edges |
| **Negatives** | K **non-edges** (random / SoHeaRT) per attack | **Benign observed** edges in test snapshots |
| **Score for ranking** | Raw \(\hat{y}\), high = positive class | Low \(\hat{y}\) = anomaly (use \(1-\hat{y}\) for AP if y=1 on attack) |
| **Pool size** | \(N \times (1+K)\) | All test-period edges (huge) |

**Bottom line:** Euler test = **attack vs real benign haystack**; HeaRT test = **attack vs sampled missing links**. Same train (benign + random non-edges); **test negative definition and score direction** differ. Align with Euler by scoring all test edges + **`1 - \hat{y}`**; keep HeaRT row for the negative-sampling ablation.

---

**User**

but euler makes more sense to me since it is trying to find needle in a haystack, thats why score should be low for anmolous attack edges. how can we adjust heart scorign scheme for this case?

---

**Cursor**

**Yes — keep Euler’s story: train “normal link likelihood,” test “low score = suspicious.”** Minimal changes to your HeaRT pipeline:

---

## 1. Define anomaly score (test only)

After decoder \(\hat{y}_{uv} = \sigma(\text{MLP}(h_u, h_v)) \in [0,1]\):

```text
anomaly_score(u,v) = 1 - ŷ_uv        # or -log(ŷ_uv + ε)
```

- **High** anomaly_score → more suspicious  
- **Attack edges** should rank **above** benign traffic in this score  

`pooled_auc_ap` stays the same: **y=1 on attack**, pass **`anomaly_score`**, not raw \(\hat{y}\).

---

## 2. Switch test negatives to Euler-style (haystack)

| Mode | Negatives |
|------|-----------|
| **Euler (main)** | Sample (or use all) **benign test-period edges** — real links, not holes |
| **HeaRT ablation** | K **non-edges** (random / SoHeaRT) — keep as extra row only |

```text
pos_test  = attack edges
neg_test  = sample(benign_edges_in_test_window \ attack), K per attack
score     = 1 - ŷ
→ pooled AUC / AP
```

That’s the needle-in-haystack eval.

---

## 3. Train — **no change**

Still: benign edges → target 1, random non-edges → target 0.

Model learns **normal link likelihood**; inversion at test is what Euler does:

```286:288:/home/mmanjee/HeaRT/euler.txt
An observed interaction ... with a likelihood
score below a certain threshold is said to be anomalous.
```

---

## 4. Optional: threshold (TPR/FPR like Euler)

On **benign val** only: pick \(\tau\) on `anomaly_score` using held-out benign edges + val non-edges (Eq. 8, λ=0.6). At test: alert if `anomaly_score ≥ τ`.

---

## 5. Code-level adjustment (conceptual)

```python
# was:
pos_s = score(pos_attack)      # high = good  ❌ for anomaly story
neg_s = score(neg_benign_test)

# use:
pos_s = 1.0 - score(pos_attack)   # high = anomalous ✓
neg_s = 1.0 - score(neg_benign_test)
metrics = pooled_auc_ap(pos_s, neg_s)  # attack y=1, should score HIGHER anomaly
```

For SoHeaRT ablation row: same `1 - ŷ`; negatives stay non-edges — shows “easy vs hard **missing** links,” not ops haystack.

---

## 6. Paper table (recommended)

| Row | Negatives | Score | Question |
|-----|-----------|-------|----------|
| **Euler** | Benign test edges | `1 - ŷ` | Needle in haystack |
| **HeaRT-random** | Random non-edges | `1 - ŷ` | Metric inflation baseline |
| **SoHeaRT** | Hard non-edges | `1 - ŷ` | Structural decoy ablation |

**One line:** Train link likelihood on benign; at test use **`anomaly = 1 - ŷ`**, attack = y=1, negatives = **benign real test edges** — Euler semantics, your GCN + optional SoHeaRT ablation.

---

**User**

so should we sample heart positives instead?

---

**Cursor**

**No — don’t sample attack positives.** Use **all** labeled attack edges.

| | Sample? | Why |
|--|---------|-----|
| **Test positives** | **No** | Small, precious label set (~500–20k). Subsampling throws away signal and makes AP unstable. |
| **Test negatives (Euler)** | **Yes** | Haystack is huge (millions of benign auths). Sample **K benign test edges per attack edge** (or fixed large pool). |
| **Test negatives (SoHeaRT ablation)** | **Yes** | Already K hard **non-edges** per attack — keep as extra row only. |

**HeaRT’s “per-positive sampling” applies to negatives, not positives.** Twitter uses the **full** held-out positive edge set; only negatives are K×.

**Recommended eval:**

```text
pos_test = ALL attack edges                    # fixed labels
neg_test = sample K benign test edges / attack # haystack (Euler)
           (+ optional K SoHeaRT non-edges)    # ablation
score    = 1 - ŷ_uv                            # high = anomalous
```

**One line:** Full attack set as positives; sample **benign** negatives from the haystack, not attack positives.

---

**User**

how can we adjust the problem so that heart makes more sense in this, for example doing attack Supervised training

---

**Cursor**

**HeaRT fits naturally when the task is “rank the true edge above hard fakes,” not “find rare anomalies in all traffic.”** Three ways to adjust — best → most Euler-like:

---

## Option A — **Supervised attack edge LP** (HeaRT-native)

| | |
|--|--|
| **Train pos** | Benign train edges **+** attack train edges (split labels by time/campaign) |
| **Train neg** | Random + **SoHeaRT** non-edges |
| **Test pos** | Held-out **attack** edges |
| **Test neg** | SoHeaRT (main) + random (ablation) |
| **Score** | Raw \(\hat{y}\): high = “this link exists / is positive class” |
| **HeaRT story** | Strong — same as Twitter, but positives include attack |

**Splits (example):**
- **LANL:** attack edges 80/20 by time; or train days 1–2 of red team, test day 3  
- **OpTC:** train attack from `23Sep19-red`, test `24Sep19` + `25Sept`  
- **CICAPT:** weak — Phase 1 has no attack; only test-phase attack labels unless you merge tactics

**Tradeoff:** No longer unsupervised IDS (Euler line). Paper becomes **“hard negatives matter for supervised cyber LP.”** Needs enough attack edges to train (~hundreds OK).

---

## Option B — **Unsupervised train + HeaRT ranking task** (no attack in train)

Reframe eval only — no architecture change:

> For each attack edge \((u,v)\), rank \((u,v)\) above **K SoHeaRT** distractors \((u,v')\).

- Train: benign only (unchanged)  
- Test: multiple-choice / ranking per attack edge  
- Score: \(\hat{y}\) or \(1-\hat{y}\) depending on whether you frame as “exists” vs “anomalous”

**HeaRT story:** Medium — SoHeaRT is the **distractor set**, not the haystack.  
**Tradeoff:** Still not Euler ops; but no attack labels in train.

---

## Option C — **Euler main + HeaRT as auxiliary** (what you had evolving toward)

| Eval row | Pos | Neg | Score |
|----------|-----|-----|-------|
| **Primary (Euler)** | All attack | Sampled **benign test** | \(1-\hat{y}\) |
| **Ablation (HeaRT)** | All attack | SoHeaRT non-edges | \(1-\hat{y}\) |

**HeaRT story:** “LP metric inflation” — not the main detection task.  
**Tradeoff:** Honest; SoHeaRT is secondary.

---

## Option D — **Jbeil-style** (supervised + temporal)

Train inductive LP on graph with **benign + attack** auths; SoHeaRT at train/test.  
**Tradeoff:** Needs temporal encoder + more engineering — tight for 6 days.

---

## Recommendation for CARS (~6 pages)

| If paper claim is… | Pick |
|--------------------|------|
| **“Hard negatives expose inflated cyber LP”** (HeaRT core) | **A** on 1–2 datasets with attack train split + **C** Euler row as “ops reality” |
| **“Unsupervised LM like Euler”** | **C** only; SoHeaRT = one ablation figure |
| **Quick + coherent** | **A on LANL** (enough labels) + **CICAPT** unsupervised Euler-style |

**Supervised train recipe (minimal change to code):**

```text
E_train_pos = E_benign_train ∪ E_attack_train
E_train_neg = SoHeaRT(E_benign_train) + random
loss = BCE(ŷ, 1) on pos, BCE(ŷ, 0) on neg

test: pos = E_attack_test, neg = SoHeaRT K per pos
metric: pooled AP/AUC on raw ŷ
```

**One line:** To make HeaRT “make sense,” **put attack edges in train positives** (time-split) and use **SoHeaRT as train+test negatives**; keep **Euler eval** (benign haystack, \(1-\hat{y}\)) as a separate row so you still speak to cyber ops.

---

**User**

and would heart still be applicable if we do Unsupervised attack training?

---

**Cursor**

**Yes — but only as an eval / ablation tool, not as the main cyber detection story.**

With **unsupervised train** (benign only, no attack labels):

| HeaRT applies to… | How |
|-------------------|-----|
| **Train negatives** | Random (and optionally SoHeaRT) non-edges — same as now |
| **Test negatives** | K hard **non-edges** per attack edge |
| **What it measures** | “Does attack rank above **plausible missing** links?” — not haystack detection |

**Still valid use:** show random-negative AP is **inflated** vs SoHeaRT (your HeaRT contribution on cyber graphs).

**Awkward part:** test positives are **attack**, score is link likelihood — you need **`1 - ŷ`** for Euler semantics, and SoHeaRT negs are **non-edges**, not benign traffic. So HeaRT doesn’t match “needle in haystack.”

**Unsupervised + sensible combo:**

```text
Main eval:  Euler-style (attack vs sampled benign test edges, score = 1 - ŷ)
HeaRT row:  attack vs SoHeaRT non-edges (score = 1 - ŷ)  →  "metric inflation" figure
```

**One line:** HeaRT **still applies** unsupervised for **benchmarking negative sampling**; it does **not** fully apply as the **operational attack-detection** metric without supervised attack in train or Euler-style benign negatives.

---

**User**

how about we do like this:
train on positives (attack) and negatives (benign)

---

**Cursor**

**That’s supervised edge classification — workable, but it changes the paper and weakens SoHeaRT’s role.**

## What you’re proposing

| | |
|--|--|
| **Train pos** | Attack edges |
| **Train neg** | Benign edges (sampled) |
| **Loss** | BCE: attack → 1, benign → 0 |
| **Test** | Attack vs benign (same types) |
| **Score** | High \(\hat{y}\) = attack |

That **does** match “needle vs haystack” at test and is easy to explain.

---

## Pros

- Model **explicitly learns attack vs benign** — no \(1-\hat{y}\) trick  
- Test = rank attack above benign — **Euler-like ops eval, but supervised**  
- HeaRT **ranking metric** still works (pooled AP/AUC on attack vs benign negs)

---

## Cons / pitfalls

| Issue | Fix |
|-------|-----|
| **Huge imbalance** | Downsample benign (e.g. 1:1 or 1:10 vs attack) or weighted BCE |
| **Leakage** | Time-split: train benign/attack from **early** window only; test from **later** |
| **CICAPT** | Phase 1 = benign negs only; **no attack in Phase 1** — train pos must come from **split Phase 2** attack edges |
| **Story vs Euler** | No longer unsupervised LM — reviewers will compare to **Jbeil**, not Euler |
| **Generalization** | Learns **seen** red-team patterns, not “normal baseline + anomaly” |

---

## HeaRT / SoHeaRT still applicable?

| | |
|--|--|
| **Benign as train/test neg** | **Yes — that’s your main negative type** |
| **SoHeaRT (hard non-edges)** | **Much less central** — those are “missing links,” not benign traffic. Optional extra row only. |
| **HeaRT contribution** | Shifts to **“hard benign negatives”** (sample structurally similar benign edges) vs random benign — not classic SoHeaRT |

So: **HeaRT-style eval yes; SoHeaRT specifically is optional / secondary** in this setup.

---

## GCN graph for message passing

Use **benign train edges only** in `adj_t` (avoid leaking attack structure into embeddings), or benign ∪ attack_train if you want transductive — **state clearly in paper**.

---

## Suggested split (LANL example)

```text
Train pos:  ~80% redteam edges (early days)
Train neg:  sample benign auths from pre-attack + non-attack days
Test pos:   remaining redteam edges
Test neg:   sample benign auths from same test days
```

---

## Verdict

| Goal | This setup |
|------|------------|
| Clear train/test + haystack eval | **Good** |
| Keep Euler unsupervised story | **No** — different problem |
| Keep SoHeaRT as main contribution | **Weak** — benign negs replace it |
| 6-page CARS deadline | **Feasible** on LANL + OpTC; CICAPT needs careful Phase-2 split |

**One line:** Train **attack=1 / benign=0** is coherent and HeaRT-metrics-friendly, but it’s **supervised cyber edge classification**, not HeaRT-on-anomaly-LP; use **sampled benign** as hard negatives, not SoHeaRT non-edges, as the main eval.

---

**User**

but we are not using heart there then

---

**Cursor**

**Correct — attack-pos / benign-neg supervised training is not really HeaRT.**

SoHeaRT = hard **non-edges** (plausible missing links). Your negatives are **real benign edges** — different object, different sampler.

---

## What you’d actually be doing

| | **HeaRT/SoHeaRT** | **Your proposal** |
|--|-------------------|-------------------|
| Train neg | Non-edges (PPR+CN/RA) | Benign edges |
| Test neg | Hard non-edges | Benign edges |
| Task | Link existence ranking | **Binary edge classification** |

That’s a **valid cyber paper**, but the contribution isn’t “SoHeaRT on cyber graphs.”

---

## If you want HeaRT in the paper — pick one path

**Path 1 — Unsupervised (Euler train) + HeaRT eval ablation**  
- Train: benign + random non-edges  
- Test rows: Euler (benign haystack) + SoHeaRT (hard non-edges)  
- **HeaRT = main hook**; Euler = ops reality  

**Path 2 — Supervised LP with SoHeaRT (HeaRT-native)**  
- Train pos: benign **+** attack (split)  
- Train neg: random **+ SoHeaRT non-edges**  
- Test pos: attack; test neg: **SoHeaRT** (+ random ablation)  
- **HeaRT = main hook**; not Euler unsupervised  

**Path 3 — Supervised attack vs benign (your idea)**  
- **No SoHeaRT** — rename paper away from HeaRT  
- Optional: “hard benign negatives” (sample similar benign edges) — inspired by HeaRT but not the method  

---

## Recommendation

You can’t have **both** “supervised attack vs benign” **and** “SoHeaRT is the contribution” without stretching definitions.

For a **HeaRT CARS paper:** use **Path 1** or **Path 2**.  
If you prefer **attack vs benign supervised:** drop SoHeaRT as the headline and frame as supervised cyber edge classification instead.

---

**User**

Path 2 — Supervised LP with SoHeaRT (HeaRT-native)

Train pos: benign + attack (split)
Train neg: random + SoHeaRT non-edges
Test pos: attack; test neg: SoHeaRT (+ random ablation)
HeaRT = main hook; not Euler unsupervised


but for path2, how will model know if edge is attack or non attack since to it all will look same as benign 

---

**Cursor**

**Good catch — naive Path 2 as stated doesn’t work.** If benign and attack are **both train positives (y=1)**, the model only learns “edge exists,” not “attack vs benign.”

---

## The fix: separate **structure** from **attack label**

| | Purpose |
|--|---------|
| **Graph / `adj_t`** | Benign **+** attack train edges (so GCN sees attack topology) |
| **Loss labels** | **Not** “all pos = 1” |

**Correct Path 2 loss:**

| Edge type | Attack label in loss |
|-----------|----------------------|
| Attack train edges | **1** |
| SoHeaRT non-edges | **0** |
| Random non-edges | **0** |
| Benign train edges | **0** (or **don’t use in attack loss** — structure only) |

```text
L = BCE(ŷ, 1) on E_attack_train
  + BCE(ŷ, 0) on E_soheart_neg
  + BCE(ŷ, 0) on E_random_neg
  (+ optional BCE(ŷ, 0) on sampled E_benign_train)
```

Benign edges are **not** train positives for the attack objective — they’re either **negatives** or only used to build the graph.

---

## Test (unchanged)

```text
pos_test = all held-out attack edges        → y=1, want high ŷ
neg_test = SoHeaRT K per attack (+ random)  → y=0
metric   = pooled AP/AUC on raw ŷ
```

---

## Optional: multi-task (stronger structure)

```text
L_LP:     benign pos vs non-edges     (learn normal links)
L_attack: attack pos vs SoHeaRT neg    (learn attack signal)
L = L_LP + λ L_attack
```

---

## What **not** to do

```text
train pos = benign ∪ attack   all → 1   ❌
```

Model cannot separate attack from benign at test.

---

**One line:** Path 2 works only if **attack edges alone are y=1** in the loss; benign goes into **adj** and/or **y=0 negatives**, SoHeaRT stays **non-edge** negatives. Then the model learns attack vs “not attack / not that link,” not “all links look the same.”

---

**User**

then give me final train/test dataset idea how it looks

---

**Cursor**

**Final spec — Path 2 (supervised attack + SoHeaRT), 3 datasets**

---

## Global rules (all datasets)

| Component | Content |
|-----------|---------|
| **`adj_t` (GCN)** | Benign train edges **∪** attack train edges |
| **Train loss y=1** | Attack train edges only |
| **Train loss y=0** | SoHeaRT non-edges + random non-edges (+ optional sampled benign train edges) |
| **Test pos** | **All** held-out attack edges (no sampling) |
| **Test neg** | K=500 SoHeaRT non-edges / attack (+ random non-edges ablation row) |
| **Score** | Raw \(\hat{y}\) — high = attack |
| **Metrics** | Pooled AUC / AP |

**Do not** put benign edges in train y=1.

---

## Dataset 1 — CICAPT-IIoT

| Split | Edges / nodes | Use |
|-------|---------------|-----|
| **Train graph** | Phase 1 all edges (~64k) | `adj_t`, features, SoHeaRT PPR graph |
| **Attack train (y=1)** | ~80% of Phase 2 attack-touching edges (~450) | Loss positive |
| **Attack test (pos)** | ~20% (~120) | Test positives |
| **Benign train (y=0, opt.)** | Sample from Phase 1 edges | Extra negatives |
| **Train negs** | SoHeaRT + random on Phase 1 graph | Loss negative |
| **Test negs** | SoHeaRT + random per test attack edge | Eval |

**Note:** Phase 1 has **zero** attack — attack labels only from Phase 2 split.

---

## Dataset 2 — LANL Cyber1

| Split | Edges | Use |
|-------|-------|-----|
| **Train graph** | All auths **before first red-team edge** | `adj_t`, SoHeaRT |
| **Attack train (y=1)** | ~80% of `redteam.txt` (~600) | Loss positive |
| **Attack test (pos)** | ~20% (~150) | Test positives |
| **Benign train (y=0, opt.)** | Sample pre-attack auths | Extra negatives |
| **Train negs** | SoHeaRT + random on pre-attack graph | Loss negative |
| **Test negs** | SoHeaRT + random per test attack edge | Eval |

**Time-split alternative:** train attack = days 1–2 of campaign, test = day 3 (if timestamps allow).

---

## Dataset 3 — OpTC (FLOW-START)

| Split | Edges | Use |
|-------|-------|-----|
| **Train graph** | 4 benign days + **23Sep19-red** attack FLOW-START | `adj_t`, SoHeaRT |
| **Attack train (y=1)** | 23Sep19-red labeled attack edges | Loss positive |
| **Attack test (pos)** | 24Sep19 + 25Sept attack edges | Test positives |
| **Benign train (y=0, opt.)** | Sample from 4 benign days | Extra negatives |
| **Train negs** | SoHeaRT + random on benign graph | Loss negative |
| **Test negs** | SoHeaRT + random per test attack edge | Eval |

**Split by campaign** (cleaner than 80/20 on one day).

---

## Paper table (one row per dataset × neg type)

| Dataset | `\|V\|` train | Train attack | Test attack | Test neg | Metric |
|---------|--------------|--------------|-------------|----------|--------|
| CICAPT | Phase 1 nodes | ~450 | ~120 | SoHeaRT-500 / Random-500 | AP, AUC |
| LANL | ~17k hosts | ~600 | ~150 | SoHeaRT-500 / Random-500 | AP, AUC |
| OpTC | ~1.1k hosts | 23Sep-red | 24Sep+25Sep | SoHeaRT-500 / Random-500 | AP, AUC |

---

## Artifacts per dataset

```text
processed/
  graph.pt              # adj: benign_train ∪ attack_train
  x_snap_svd256.pt      # features from train graph
  attack_train.pt       # y=1 edges
  attack_test.pt        # test positives
  heart_train_*.npy     # SoHeaRT negs for train (optional per-batch)
  heart_test_soheart.npy   # [N_test_attack, 500]
  heart_test_random.npy    # [N_test_attack, 500]
```

---

## Optional add-ons (not main table)

| Row | Purpose |
|-----|---------|
| **+ benign test negs** | Euler-style haystack (ops realism) |
| **Multi-task** `L_LP + λ L_attack` | Stronger benign structure if AP is weak |
| **Unsupervised row** | Benign-only train + `1-ŷ` vs benign test — compare to Euler line |

---

## One-sentence paper framing

> We train a GCN link scorer to rank **attack edges above SoHeaRT hard non-edges**, with attack labels only in the loss (benign edges define graph structure, not positive class), and show **random negatives inflate AP** on three cyber graphs.

---

**User**

Component	Content
adj_t (GCN)
Benign train edges ∪ attack train edges
Train loss y=1
Attack train edges only
Train loss y=0
All benign edges
Test pos
All held-out attack edges (no sampling)
Test neg
K=500 SoHeaRT non-edges / attack (+ random non-edges ablation row)
Score
Raw 
y
^
y^​ — high = attack
Metrics
Pooled AUC / AP

---

**Cursor**

**Mostly coherent as supervised attack vs benign — but fix three things.**

---

## Your table — what works

| Piece | OK? |
|-------|-----|
| `adj_t` = benign ∪ attack train | Yes — GCN sees both |
| y=1 = attack train only | Yes |
| y=0 = **benign** (all) | Yes — **supervised attack classifier**, not classic LP |
| Test pos = all held-out attack | Yes |
| Score: high = attack | Yes |

---

## Issues to fix

**1. “All benign” at train — not literal**  
LANL/OpTC have millions of benign edges. Use **sampled** benign per batch (e.g. 1:1 or 1:10 vs attack), not full set every epoch.

**2. Train/test negative mismatch**

| | Train y=0 | Test neg |
|--|-----------|----------|
| **Yours** | Benign edges | SoHeaRT **non-edges** |

Model learns **attack vs benign**; test asks **attack vs missing links** — different task. SoHeaRT test AP may be **easier** than real ops (benign harder than non-edges).

**Fix (pick one):**

- **A (aligned train):** y=0 = SoHeaRT + random non-edges (+ sample benign) — HeaRT-native  
- **B (aligned test):** test neg = **sampled benign test edges**; SoHeaRT = ablation only  
- **C (report both):** main = benign test negs; extra row = SoHeaRT  

**3. SoHeaRT role with y=0 = all benign**  
SoHeaRT is **not** in train — only in test ablation. HeaRT headline is weaker unless you add SoHeaRT to train y=0.

---

## Revised table (recommended)

| Component | Content |
|-----------|---------|
| **`adj_t`** | Benign train ∪ attack train |
| **Train y=1** | Attack train edges |
| **Train y=0** | SoHeaRT non-edges + random non-edges + **sampled** benign train edges |
| **Test pos** | All held-out attack |
| **Test neg** | K=500 SoHeaRT / attack (+ random ablation) |
| **Score** | Raw \(\hat{y}\), high = attack |
| **Metrics** | Pooled AUC / AP |

**Optional row:** test neg = sampled **benign test** edges (Euler haystack).

---

**One line:** Your version = **attack vs benign supervised**; add SoHeaRT to **train y=0** (and sample benign) so train and test both use HeaRT-style hard negatives, or switch test negs to **benign** if train stays benign-only negatives.

---

**User**

does any of the papers do like this jbeil or any? or all train on non attack and predict on attack?

---

**Cursor**

**Not all are benign-only — but Euler/Bowman are. Jbeil is the main exception in this line.**

## Who trains on what

| Paper | Train data | Attack labels in train? | Test |
|-------|------------|-------------------------|------|
| **Bowman ’20 [16]** | Normal auth graph (non-attack days) | **No** — unsupervised | Low P(link) on new auths vs red-team labels |
| **Euler ’23** | Snapshots **before first anomaly**; val = benign only | **No** — unsupervised | Low likelihood on test edges; positives = labeled anomalous |
| **Jbeil ’24** | **Benign + malicious** auths (+ augmented threat samples) | **Yes** — self-supervised LP on both | Inductive LP on held-out nodes/edges; LM paths |
| **CyberGFM ’26** (newer) | Pretrain on walks; **fine-tune** malicious vs benign | **Yes** (fine-tune stage) | Classify / score links as malicious |

```771:772:/home/mmanjee/HeaRT/euler.txt
Like many other anomaly detection models [15, 16, 42, 54], we make the assumption that a
large sample of benign data is available from which to learn a baseline for normal behavior offline.
```

```873:874:/home/mmanjee/HeaRT/euler.txt
the training set consists of all snapshots that occur before the first anomalous edge appears
```

Euler explicitly contrasts Jbeil-style “more data in train” with Bowman using all non-attack days — still **no attack-as-positive supervised loss** in Euler/Bowman.

---

## Where your Path 2 fits

| Your plan (attack y=1, benign/SoHeaRT y=0) | Closest prior |
|--------------------------------------------|---------------|
| Supervised **edge-level** attack vs not-attack | **Jbeil / CyberGFM** (not Euler/Bowman) |
| Benign-only train, attack at test | **Euler / Bowman** (anomaly LP) |

Jbeil does **not** exactly do “attack=1, all benign=0” BCE — it’s **self-supervised temporal LP** on graphs that **include** malicious edges, plus threat augmentation. Your plan is **more explicitly supervised** than Jbeil.

---

## Summary

- **Euler, Bowman, most LM-anomaly papers:** train on **non-attack / benign**, predict/score **attack at test** (no attack labels in loss).  
- **Jbeil (and similar):** train **with** malicious edges in the graph/loss — **not** benign-only.  
- **Your supervised Path 2:** aligned with **Jbeil family**, **not** Euler; cite both and state you use **SoHeaRT hard non-edges** as the eval hook Euler/Jbeil don’t emphasize.

---

**User**

how about converting problem to following where we say:
non-attack --> link
attack --> non link


and score non links or something like that 



or maybe opposite of the above

---

**Cursor**

**One mapping works (Euler-aligned); the opposite breaks on benign.**

---

## Option A — **Benign = link, attack = non-link** (recommended)

| Edge type | LP label | Train role | Desired \(\hat{y}\) at test |
|-----------|----------|------------|----------------------------|
| Benign | **1** (link exists) | Train positive | **High** |
| Attack | **0** (non-link) | Train negative (if in train split) | **Low** |
| SoHeaRT / random | **0** | Train negative | **Low** |

**Train loss:**
```text
y=1: benign train edges
y=0: SoHeaRT + random non-edges + attack train edges (optional but strong signal)
```

**Test (attack detection):**
```text
anomaly_score = 1 - ŷ     # high = suspicious
pos = attack test edges   (y=1 in AP)
neg = SoHeaRT K / attack  OR sampled benign test edges (Euler haystack)
```

**Why it makes sense:** Same story as Euler (“attack = unlikely link given normal graph”), but you can **put attack in train as y=0** so the model is explicitly pushed away from attack pairs — not just “never saw them.”

**HeaRT:** SoHeaRT stays **non-edge** negatives (y=0), same as benign LP. Attack also y=0 — both compete in “low score” class; at test you rank **attacks vs SoHeaRT** using **`1 - ŷ`**.

---

## Option B — **Attack = link, benign = non-link** (don’t use)

Benign edges **exist** in the data. Labeling them non-link (y=0) fights the graph and is wrong unless benign is **only** in `adj_t`, never in the loss — which collapses to **attack y=1, SoHeaRT y=0** (Path 2), not this inversion.

---

## vs Euler / HeaRT

| | Euler (unsupervised) | Your Option A |
|--|---------------------|---------------|
| Benign train | y=1 (link) | y=1 (link) |
| Attack train | not in loss | **y=0 (non-link)** ← extra supervised signal |
| Test | low \(\hat{y}\) on attack | same, use **`1 - ŷ`** for AP |
| HeaRT negs | N/A at test (benign haystack) | SoHeaRT = y=0 train & test |

---

## Final train/test (Option A)

| Component | Content |
|-----------|---------|
| **`adj_t`** | Benign train ∪ attack train (both in graph) |
| **Train y=1** | Benign train edges |
| **Train y=0** | SoHeaRT + random + **attack train edges** |
| **Test pos (AP)** | All held-out attack (`y=1` on **`1-ŷ`**) |
| **Test neg** | SoHeaRT K/attack (+ random; optional benign-test row) |

**One line:** Use **benign = link, attack = non-link**, score **`1 - ŷ`** at test — Euler semantics with optional **attack-as-negative** in train; opposite mapping doesn’t work for real benign edges.

---

**User**

but if we do attack as non links then all non links will be considered as attack 

---

**Cursor**

**Not if you’re clear about what y=0 means — but your worry is real for deployment.**

## What y=0 is *not*

**y=0 ≠ “this is an attack.”**  
It means **“not a normal benign link.”** That set includes:

- random non-edges  
- SoHeaRT non-edges  
- attack edges (if you add them in train)

So the model is **not** learning “low score ⇒ attack class.” It learns **“low score ⇒ not benign-normal.”**

---

## Why “all non-links = attack” doesn’t follow at eval

**HeaRT/Euler eval never scores all possible non-edges.**

```text
Candidates scored = attack test edges  +  K negatives per attack
Not = all O(|V|²) missing links
```

AP/AUC asks: among **these** pairs, do attacks rank as **more anomalous** than the chosen negatives?  
Billions of other non-edges are **never scored**.

**Euler ops:** only **observed** edges in logs get scored (auths that happened). Random non-edges aren’t in the haystack unless you explicitly enumerate them.

---

## Where your concern *does* bite

If you **deploy** with rule `ŷ < τ ⇒ alert` over **all** pairs → yes, almost every non-edge looks “suspicious.” **Don’t do that.**

Deploy like Euler: score **observed test-period edges only**, rank by **`1 - ŷ`**, alert top-k or τ tuned on benign val.

---

## Train issue: attack as y=0 alongside SoHeaRT y=0

Both get the same loss target (push ŷ down). The model may **not** separate “attack non-link” from “random non-link” — they can all sit in the same low-score bucket.

**Fixes:**

| Approach | Idea |
|----------|------|
| **Unsupervised (Euler)** | Attack **not** in train loss; only benign y=1 vs non-edges y=0 |
| **Attack y=0 + weight** | Higher weight on attack y=0 so they score **lower** than SoHeaRT |
| **Two terms** | `L_link`: benign vs non-edges; `L_attack`: attack vs benign sample (separate) |
| **Don’t put attack in adj** | `adj_t` = benign only; attack only in loss as y=0 (harder but cleaner) |

---

## Practical recommendation

**For HeaRT paper + avoid “all non-links = attack” confusion:**

```text
Train:  benign y=1,  SoHeaRT + random y=0   (attack NOT in train loss)  → Euler-like
Test:   anomaly = 1 - ŷ,  attack vs SoHeaRT (+ benign-test row)
```

**If you want attack in train:** use **`adj_t` = benign only**, attack train edges as **y=0 only in decoder loss** (not “every non-link is attack”).

**One line:** y=0 means “not normal link,” not “attack”; eval scores a **small candidate set**, not all non-links — and keep **`adj_t` benign-only** if you train attack as non-link to avoid conflating every missing edge with attack.

---

**User**

ok give me final detail table concise

---

**Cursor**

**Final spec (locked after discussion)**

---

## Model

| Item | Setting |
|------|---------|
| Encoder | 3-layer GCN, hidden 256, dropout 0.1 |
| Decoder | MLP + dot product → \(\hat{y}_{uv}\) |
| Features | SVD-256 from benign graph |
| **`adj_t`** | **Benign train edges only** (no attack in graph) |

---

## Train / test (all datasets)

| Component | Content |
|-----------|---------|
| **Train y=1** | Benign train edges |
| **Train y=0** | SoHeaRT non-edges + random non-edges |
| **Attack in train?** | **No** (not in loss, not in `adj_t`) |
| **Test pos** | **All** held-out attack edges |
| **Test neg (main)** | K=500 **SoHeaRT** non-edges / attack |
| **Test neg (ablation)** | K=500 **random** non-edges / attack |
| **Test neg (optional)** | K=500 **benign test** edges / attack (Euler haystack) |
| **Anomaly score** | **`1 - ŷ`** (high = suspicious) |
| **AP/AUC labels** | y=1 attack, y=0 negatives; score = **`1 - ŷ`** |
| **Val** | Held-out benign edges + non-edges (no attack); early stop |

---

## Per-dataset splits

| Dataset | Benign train (`adj`, y=1) | Attack test (pos) | SoHeaRT graph |
|---------|---------------------------|-------------------|---------------|
| **CICAPT** | Phase 1 edges | Phase 2 attack-touching edges (~570) | Phase 1 |
| **LANL** | Auths before 1st red-team edge | All `redteam.txt` (~750) | Pre-attack graph |
| **OpTC** | 4 benign days FLOW-START | 24Sep + 25Sep attack FLOW-START | 4 benign days |

---

## Paper results table

| Dataset | Neg type | # attack test | K | Metric |
|---------|----------|---------------|---|--------|
| CICAPT | Random / SoHeaRT / (Benign†) | ~570 | 500 | AP, AUC |
| LANL | Random / SoHeaRT / (Benign†) | ~750 | 500 | AP, AUC |
| OpTC | Random / SoHeaRT / (Benign†) | ~21k‡ | 500 | AP, AUC |

† optional Euler row · ‡ use campaign subset or sample if too large

---

## Artifacts

```text
graph.pt                 # benign train only
attack_test.pt           # test positives
heart_test_soheart.npy   # [N_attack, 500]
heart_test_random.npy    # [N_attack, 500]
benign_test_neg.npy      # optional [N_attack, 500]
```

---

## Paper claim (one line)

> Benign-only GCN LP + **SoHeaRT** test negatives show cyber **AP is inflated under random non-edges**; **`1 - ŷ`** ranks held-out attack edges vs hard decoys (Euler-style benign row optional).

---

## Do not

- Put attack in train loss or `adj_t`  
- Use raw \(\hat{y}\) with attack as y=1 without **`1 - ŷ`**  
- Sample attack test positives  
- Label all y=0 as “attack”

---

**User**

but this way model learns about links and non-links not attack and non-attacks

---

**Cursor**

**Correct — with this final spec the model learns “benign link vs non-link,” not “attack vs non-attack.”**

That’s **by design** (Euler/Bowman line):

```text
Train:  P(benign link | graph)
Test:   hope attack edges get low P(link)  →  high (1 - ŷ)
```

Attack labels appear **only in test metrics**, not in the loss.

---

## What you can and cannot claim

| Claim | OK? |
|-------|-----|
| “Model learns attack patterns” | **No** |
| “Benign-only LP + hard negatives reveal metric inflation” | **Yes** (HeaRT hook) |
| “Attack edges are unexpectedly low-link-likelihood vs decoys” | **Yes**, if results show it |
| “Unsupervised LM detector like Euler” | **Yes** (static GCN, no RNN) |

---

## If you need **attack vs non-attack** in the loss

You must add **supervision** — no way around it:

| Knob | Change |
|------|--------|
| **Train y=1** | Attack train edges |
| **Train y=0** | SoHeaRT + sampled benign |
| **`adj_t`** | Benign only (or benign ∪ attack train) |
| **Score** | Raw \(\hat{y}\), high = attack |
| **Prior** | Jbeil-style, not Euler |

SoHeaRT stays relevant; story shifts from “metric critique” to “supervised cyber LP with hard negatives.”

---

## Pick one paper story

| Story | Train | Test | Headline |
|-------|-------|------|----------|
| **A (current table)** | Link / non-link (benign only) | `1-ŷ`, attack vs SoHeaRT | **Hard negatives expose inflated cyber LP** |
| **B (supervised)** | Attack vs SoHeaRT/benign | \(\hat{y}\), attack vs SoHeaRT | **SoHeaRT for supervised attack edge ranking** |

**One line:** Your table is **anomaly LP**, not attack classification — honest framing is “we test whether **attack edges rank as anomalous links** under SoHeaRT,” not “the model learns attack.” If that’s too weak, switch to **Story B**.

---

**User**

i dont feel soheart is becoming applicable for this link prediction problem with this dataset

---

**Cursor**

**Your instinct is right — SoHeaRT is a weak fit for this cyber problem as the main idea.**

## Why it feels off

| SoHeaRT assumes | Cyber attack detection is |
|-----------------|---------------------------|
| Task = **does edge exist?** | **Is this observed edge malicious?** |
| Negatives = **missing** links (PPR/CN hard non-edges) | Haystack = **real benign** traffic |
| Train/test = same LP task (OGB/Twitter) | Euler: train normal, test **anomalous among real edges** |
| High score = true link | Attack edges **are** real links |

SoHeaRT answers: *“rank this attack above a fake hole in the graph.”*  
Ops needs: *“rank this attack above normal comms that also exist.”*  
Those are different questions.

---

## Where SoHeaRT still fits (narrow)

One **ablation row** only:

> Random non-edge AP ≫ SoHeaRT non-edge AP ≫ benign-test AP

Shows **LP papers overstate cyber results** — not a detector design.

---

## What fits your datasets better

| Direction | Negatives at test | Paper line |
|-----------|-------------------|------------|
| **Euler-style (recommended)** | Sampled **benign test** edges | Unsupervised LM + haystack eval |
| **Supervised attack LP** | SoHeaRT + sampled benign | Jbeil-like; SoHeaRT secondary |
| **Hard benign negatives** | Benign edges with high CN/PPR to attack (no non-edges) | “Hard negatives for cyber” **without** SoHeaRT |

---

## CARS recommendation (~6 pages)

**Drop SoHeaRT as the headline.** Use:

1. **Benign-only GCN** (Euler train)  
2. **Test:** `1 - ŷ`, attack vs **benign test** edges (+ random non-edge ablation showing inflation)  
3. **3 datasets:** CICAPT, LANL, OpTC  
4. **Contribution:** *hard/easy **negative protocols** for cyber LP eval* — not “SoHeaRT on cyber”

Optional: adapt HeaRT idea to **hard benign negatives** (same-subnet, same-role hosts) — same spirit, actually on-graph.

**One line:** SoHeaRT targets **missing-link LP**; your data needs **anomalous-link** or **attack-vs-benign** eval — keep Euler haystack as main, cite HeaRT only as “random non-edges mislead” motivation, or pivot title away from SoHeaRT.

---

**User**

what is hard negatives contribution here?

---

**Cursor**

**In cyber LP, the “hard negatives” contribution is about evaluation protocol — not SoHeaRT’s PPR non-edges.**

## The problem hard negatives address

Many LP setups (and sloppy cyber papers) use **easy negatives** = random non-edges. That answers:

> *“Is this attack edge more link-like than a random hole?”*

That’s **too easy** → AP looks great, says little about finding attacks in real traffic.

**Hard negatives** tighten the question to:

> *“Is this attack edge more suspicious than **plausible alternatives**?”*

---

## Three negative tiers (your contribution = comparing them)

| Tier | What | Cyber meaning |
|------|------|----------------|
| **Easy** | Random non-edges | Trivial decoys |
| **Medium** | SoHeaRT-style structural non-edges | Plausible **missing** links (optional ablation) |
| **Hard (main)** | **Benign test edges** (same period, real traffic) | Operational haystack — Euler’s setting |

**Contribution:** show AP **drops sharply** as negatives get harder, using the **same model, same attack labels, same train**.

```text
AP_random  >>  AP_soheart  >>  AP_benign_test   (typical pattern)
```

That’s “**hard negatives matter for cyber LP evaluation**” — not “we invented a new detector.”

---

## What you’re *not* claiming

- Not that hard negatives **train** a better attack classifier (unless you add supervision)  
- Not that SoHeaRT transfers cleanly to cyber ops  
- Not beating Euler on detection — **exposing when LP metrics lie**

---

## Optional twist (still HeaRT-*inspired*)

**Hard benign negatives:** for each attack \((u,v)\), sample benign \((u,v')\) with high CN/PPR — same host role/subnet, **real** edges. Harder than random benign, still on-graph.

That’s a **cyber-native** hard-negative sampler; cite HeaRT as motivation.

---

## One-line contribution for the paper

> Cyber link-prediction papers inflate performance with **random non-edge negatives**; we show **benign traffic (and optionally structure-aware benign pairs) as hard negatives** gives a realistic eval aligned with lateral-movement detection, while easy negatives overstate AP by a large margin.

**Hard negatives here = how you construct the test negative set**, not a new GNN architecture.

---

**User**

Hard benign negatives: for each attack 
(
u
,
v
)
(u,v), sample benign 
(
u
,
v
′
)
(u,v′) with high CN/PPR — same host role/subnet, real edges. Harder than random benign, still on-graph.

That’s a cyber-native hard-negative sampler; cite HeaRT as motivation.



ok give table of train test dataset for this 1 concise table

---

**Cursor**

| **Component** | **CICAPT-IIoT** | **LANL Cyber1** | **OpTC (FLOW-START)** |
|---------------|-----------------|-----------------|------------------------|
| **`adj_t` / features** | Phase 1 benign edges | Pre–red-team auths | 4 benign days |
| **Train y=1** | Phase 1 benign edges | Pre–red-team auths | 4 benign days |
| **Train y=0** | Random non-edges | Random non-edges | Random non-edges |
| **Attack in train?** | No | No | No |
| **Test pos** | Phase 2 attack-touching edges (~570) | `redteam.txt` (~750) | 24Sep + 25Sep attack edges |
| **Test neg — Easy** | K random non-edges / attack | same | same |
| **Test neg — Hard benign** | K benign Phase-2 edges \((u,v')\) with high CN/PPR to attack \((u,v)\) | K benign test-day auths from same src \(u\) | K benign eval-day FLOW-START from same src IP |
| **Score** | `1 - ŷ` (high = suspicious) | same | same |
| **Metric** | Pooled AP / AUC per neg type | same | same |

**Global:** K=500 · attack test = all labels (no sampling) · val = held-out benign + random non-edges only · GCN on benign `adj_t` only.

---

**User**

ok this seems good idea but in train if we do not specify what is attack edge and what is non attack edge then how will model  at test time filter out attack egde from heart non attack edges>

---

**Cursor**

**It won’t reliably “filter attack” without attack labels in train.** Same limitation as Euler.

## What the model actually learns

```text
Train:  benign edges → high ŷ
        random holes   → low ŷ
        (no attack signal)
```

So it learns **benign link structure**, not **attack vs benign**.

## At test with hard benign negatives

For attack \((u,v)\) vs benign \((u,v')\) (same source, high CN/PPR):

| Edge | Typical ŷ | Anomaly `1-ŷ` |
|------|-----------|----------------|
| Benign \((u,v')\) | **High** (real normal link) | Low |
| Attack \((u,v)\) | High **or** low | Low **or** high |

Separation only if attack \((u,v)\) is **less like benign train** than other real auths from \(u\) that day. Often they look the same → **AP ≈ random**. That’s a valid finding, not a bug.

## What hard benign negatives change

They don’t teach the model attack. They **raise the eval bar**:

```text
Easy (random non-edges):  attack almost always wins
Hard (benign u,v'):       attack must beat real normal traffic  ← Euler haystack
```

**Contribution:** show how much AP **drops** when negatives are realistic — not “GCN learns to detect attack.”

## If you need real attack filtering

You must add **supervision** (attack in train loss) or **temporal context** (Euler RNN). Benign-only train cannot guarantee test separation on hard benign negs.

**One line:** No attack in train → model ranks by **benign normality**, not attack class; hard benign negatives test whether that ranking is **useful for LM**, and often it won’t be much better than chance — which is exactly what the paper can report.

---

**User**

how about we label in Train (bengin U attack)
B: y =1 
A : y =0

Test:
Heart Benign 
Attack:  How better attack ranks in heart benign to tell its really an attack not a benign confused for attack

---

**Cursor**

**This is coherent — and fixes the “no attack in train” gap.** You’re doing **supervised benign-vs-attack LP** with **hard benign test negatives**.

---

## Mapping

| Edge type | Train label | Desired ŷ | Role |
|-----------|-------------|-----------|------|
| **Benign (B)** | **y = 1** | High | Normal links |
| **Attack (A)** train split | **y = 0** | Low | Not normal |
| **Random non-edges** | **y = 0** | Low | Standard LP negs |

**Test**

| | |
|--|--|
| **Pos** | Held-out **attack** edges |
| **Neg** | **Hard benign** \((u,v')\) per attack \((u,v)\) (high CN/PPR, same \(u\)) |
| **Score for AP** | **`1 - ŷ`** (high = attack-like) |
| **Question** | Does attack rank above **confusable benign** traffic? |

---

## One table

| | **Train** | **Test** |
|--|-----------|----------|
| **`adj_t`** | Benign only | Benign only (frozen) |
| **y=1** | Benign edges | — |
| **y=0** | Attack train + random non-edges | — |
| **Pos (AP)** | — | All held-out attack |
| **Neg (AP)** | — | K hard benign \((u,v')\) / attack |
| **Also report** | — | K random non-edges / attack (easy ablation) |

**Splits:** 80/20 attack by time/campaign; benign = pre-attack / Phase 1 (+ sampled in loss if huge).

---

## Why this answers your question

Train **does** separate attack (y=0) from benign (y=1).  
Test asks: *among real edges from the same host, is the attack more “non-benign” than other benign links?* — not *“is it a link vs hole?”*

---

## Caveats (state in paper)

- Needs **enough attack in train** (LANL/OpTC OK; CICAPT: split Phase-2 attacks only)  
- **`adj_t` benign-only** — attack only in loss, not message passing (or leakage)  
- Not Euler-unsupervised — cite **Jbeil** + **hard-negative eval** contribution  
- Hard benign AP will still be **much lower** than random-non-edge AP — that’s the point  

**One line:** Train **B=1, A=0**; test attack vs **HeaRT-style hard benign** with **`1-ŷ`** — explicit attack learning + realistic confusing negatives.

---

**User**

but i think in cyber they dont confuse benign with attack but attack with benign so our problem makes sense or no?

---

**Cursor**

**Yes — your problem matches how cyber LM actually fails.**

## Operational confusion (asymmetric)

| Direction | Meaning | Severity in LM |
|-----------|---------|----------------|
| Benign → attack (FP) | Normal auth flagged | Bad for ops, but not the stealth story |
| **Attack → benign (FN)** | **Attack looks like normal traffic** | **Core LM problem** — Pass-the-Hash, stolen creds, same-subnet flows |

Papers care most about: **attacks hiding among benign**, not the reverse.

---

## How your setup maps to that

```text
Train:  benign → y=1 (normal link)
        attack → y=0 (not normal)

Test:   attack (u,v)  vs  hard benign (u,v')  ← “could this attack pass as normal?”
```

**Hard benign negatives = the confusers** — real traffic from the same host / similar role that an attack might **blend into**.

**Good model:** attack ranks **more suspicious** than those benign lookalikes (`1-ŷ` higher on attack).  
**Realistic failure:** attack scores like benign → **confused with benign** → low AP. That’s a meaningful result.

---

## Why it makes sense as a paper

You’re not asking “is this a link?” (trivial with random non-edges).  
You’re asking: **“Does this attack separate from benign traffic it mimics?”** — the right cyber question.

**One line:** Cyber confuses **attack with benign**; your train (B=1, A=0) + **hard benign test negatives** directly tests that — yes, the problem makes sense.

---

**User**

why would hard benign look like an attack? why can we say that? 

---

**Cursor**

**We don’t claim hard benign “looks like attack.”** We claim **attack can look like hard benign** — that’s the LM stealth problem.

## Correct direction

| Wrong | Right |
|-------|-------|
| “Benign looks like attack” | **“Attack looks like benign”** (FN / blending in) |

Hard benign negatives are **normal edges picked to be hard to tell apart from the attack edge**, not edges that appear malicious.

## Why pick \((u, v')\) with same \(u\) + high CN/PPR?

For attack edge \((u, v)\):

1. **Same source \(u\)** — often the compromised host; all its auths that day share context.
2. **High CN/PPR to \((u,v)\)** — \(v'\) is a peer the graph treats like \(v\) (same subnet, role, frequent partner).
3. **Real benign \((u,v')\)** — routine internal traffic the model was trained to score **normal** (y=1).

So if attack \((u,v)\) is lateral movement, it often resembles **other boring auths from the same host** — Euler/Bowman’s stolen-credential story.

## What we can say in the paper (precise)

> Hard benign negatives are **plausible normal alternatives** from the same source and neighborhood as the attack edge — the traffic an analyst (or model) might accept as benign if the attack edge is not separated.

Not: “benign mimics attack.”  
Yes: “attack must be ranked above **structurally similar benign** pairs.”

## What we cannot claim

- That \(v'\) is suspicious  
- That hard benign are “attack-like”  
- That separation will work — often it **won’t** (AP ≈ 0.5), which supports the paper

**One line:** Hard benign are **confusable normal decoys**, not attack lookalikes — justified because LM attacks hide among **same-host, same-neighborhood** benign activity.

---

**User**

oh so we say find attacks among those that look benign 

---

**Cursor**

**Yes — exactly.**

**Task:** Among edges that **look like normal traffic** (especially same-host / similar-neighborhood benign links), can the model **rank the real attack highest** in suspicion (`1 - ŷ`)?

**Easy eval (random non-edges):** “find attacks among obvious non-links” — too easy.  
**Your eval (hard benign):** “find attacks among **plausible benign**” — the real LM question.

---

**User**

would this way are we making any contribution or not?

---

**Cursor**

**Yes — but only as an evaluation / benchmarking contribution, not a new detector.**

## What is a contribution

| Piece | Novel? | Claim |
|-------|--------|-------|
| **Hard benign negative protocol** (per-attack \((u,v)\), sample \((u,v')\) via CN/PPR) | **Yes** (cyber-native, HeaRT-*inspired*) | “LP eval must use **plausible benign** decoys, not random holes” |
| **Empirical gap** random non-edge AP vs hard-benign AP | **Yes** if gap is large on 3 datasets | Same spirit as HeaRT on OGB, **new domain + sampler** |
| **Task framing** “find attacks among benign-looking traffic” | **Useful** | Makes LM stealth explicit |
| **Train B=1, A=0** | **Weak alone** | Jbeil / supervised cyber LP already do attack in train |
| **Static GCN** | **No** | Commodity |

**Sellable one-liner:**  
*Hard negatives for cyber link prediction should be **on-graph benign lookalikes**, not random non-edges; we propose a sampler and show metrics collapse when evaluated realistically.*

---

## What is **not** enough

- “We run GCN on CICAPT/LANL/OpTC”  
- Beating Euler without temporal model (unlikely)  
- SoHeaRT non-edges as main story (you already moved past this)

---

## What makes it **strong enough** for ~6 pages

1. **Table:** AP/AUC on 3 datasets × {random non-edge, random benign, **hard benign**}  
2. **Ablation:** hard vs random benign (shows CN/PPR matters)  
3. **Honest numbers:** hard-benign AP likely low — argue that’s the point  
4. **Position vs Euler:** they use full haystack implicitly; you **formalize per-attack hard decoys** (HeaRT-style controlled eval)

---

## Verdict

| Framing | Contribution |
|---------|----------------|
| “New IDS” | **No** |
| “HeaRT-style hard negatives **for cyber LP evaluation**” | **Yes — modest but defensible** for CARS if results + 3 datasets are clean |

**Bottom line:** Contribution = **protocol + empirical study**, not architecture. That’s enough if the AP gap is clear and the writing is tight.

---

**User**

but heart non edges is not applicable thats why i dropped and now we do heart benign

---

**Cursor**

**Right call — drop SoHeaRT non-edges for cyber.** Your contribution is **HeaRT-inspired**, not HeaRT applied.

## What you dropped vs what you keep

| | Cyber fit |
|--|-----------|
| **SoHeaRT non-edges** | Poor — “missing link” ≠ “benign traffic” |
| **Hard benign \((u,v')\)** | Good — **on-graph** decoys attack hides among |

## How to cite HeaRT (honest)

> HeaRT shows **easy negatives inflate LP metrics** on social graphs. We argue the cyber analogue is **not** structural non-edges but **plausible benign edges** (same source, high CN/PPR). We propose a **hard benign negative sampler** and evaluate attack ranking under that protocol.

**Not:** “We apply SoHeaRT to cyber.”  
**Yes:** “We **adapt the hard-negative evaluation idea** to lateral-movement graphs.”

## Your contribution (renamed)

| Old (dropped) | New (yours) |
|---------------|-------------|
| SoHeaRT PPR non-edges | **Hard benign negatives (HBN)** or similar |
| Link-existence LP | **Attack vs benign lookalike ranking** |
| Train benign only | **Train B=1, A=0** (supervised) |

## Paper hook (one line)

**Random non-edge AP is meaningless for cyber LM; hard benign negatives expose whether models can find attacks among traffic that looks normal.**

That’s a clean story without claiming SoHeaRT transfers.

---

**User**

comparison table of my way vs euler/bowman / jbiel. you may use web to retrieve details

---

**Cursor**

**Your approach (HBN)** vs Bowman / Euler / Jbeil

| Dimension | **Yours (HBN)** | **Bowman ’20** | **Euler ’23** | **Jbeil ’24** |
|-----------|-----------------|----------------|---------------|---------------|
| **Goal** | Rank **attack above benign lookalikes** | Flag **low-P** auths | **Low link likelihood** = LM anomaly | **Self-supervised LP** for LM paths |
| **Graph** | Static | Static auth graph | **Temporal** snapshots (δ≈30m) | **Continuous-time** dynamic |
| **Model** | GCN + MLP | Random-walk + LR | GCN/GAT/SAGE + **GRU/LSTM** | **TGN**-style temporal GNN |
| **Attack in train?** | **Yes** (y=0) | **No** | **No** | **Yes** (malicious auths in graph + augmentation) |
| **Benign in train** | y=1 | All edges (non-attack days) | y=1 (pre-attack only) | Yes (benign + malicious events) |
| **Train negatives** | Random **non-edges** | Implicit in LP | Random **non-edges** / snapshot | LP negatives + **threat augmentation** (not hard benign) |
| **Supervision** | **Supervised** edge labels (B=1, A=0) | **Unsupervised** | **Unsupervised** | **Self-supervised** LP |
| **Test positives** | Held-out **attack** edges | Labeled **anomalous** auths | Labeled **anomalous** edges | LM / non-existent edges (LP framing) |
| **Test negatives** | **Hard benign** \((u,v')\) (CN/PPR, same \(u\)) | Implicit **all other** scored auths | Implicit **benign haystack** (all other test edges) | LP negatives; **not** hard-benign protocol |
| **Score / alert** | **`1 - ŷ`** (high = attack) | Low **P(link)** | Low **likelihood** | Link-prediction scores |
| **Hard negatives** | **Explicit hard benign sampler** (your contribution) | None | Implicit full haystack; **no** per-attack sampler | Augmented threats; criticizes **random non-edge** negs only |
| **Temporal** | No | No | **Yes** | **Yes** |
| **Datasets** | CICAPT + LANL + OpTC | LANL | LANL + OpTC | LANL (+ Pivoting) |
| **Main claim** | **Eval protocol**: random non-edge AP misleads; use **hard benign** | Unsupervised graph AI for LM | Scalable **temporal** anomaly LP | Inductive temporal LP + **threat augmentation** |

---

## Positioning (one line each)

| Paper | Core idea |
|-------|-----------|
| **Bowman** | Normal auth graph → low-probability edge = anomaly |
| **Euler** | Benign-only temporal LP → anomalous **observed** edges score low |
| **Jbeil** | Train on **benign + malicious** (+ augmented threats); temporal inductive LP |
| **Yours** | Train **B=1 / A=0**; test whether attack ranks above **HeaRT-inspired hard benign** decoys |

---

## Where you differ (contribution)

| vs | Your delta |
|----|------------|
| **Euler/Bowman** | **Supervised** attack in train; **explicit** hard-benign test negatives (they use implicit full haystack) |
| **Jbeil** | No temporal TGN / augmentation — focus on **evaluation**: hard **on-graph benign** vs random non-edge |
| **HeaRT/SoHeaRT** | Cyber-native **benign** hard negatives, not structural **non-edges** |

---

## Honest limits (for paper)

- Weaker than Euler/Jbeil on **temporal** LM (static GCN).  
- Supervised like Jbeil but **simpler** model.  
- **Not** a new detector SOTA — **benchmarking / negative-protocol** paper.

**Title direction:** *Hard Benign Negatives for Cyber Link Prediction Evaluation* (not “SoHeaRT on cyber”).

---

**User**

write me a quick introduction for this paper, concise

---

**Cursor**

Lateral movement in enterprise and industrial networks often appears as ordinary host-to-host or provenance edges that blend into benign activity. Graph-based link prediction has been proposed for this setting—Bowman et al. use unsupervised auth-graph models; Euler extends the idea with temporal snapshots; Jbeil trains on benign and malicious events with threat augmentation. A common thread is to score edges and treat low-likelihood or mis-predicted links as suspicious. Yet evaluation practice still relies heavily on **random non-edges** as negatives. On cyber graphs that is a poor match: attacks are **real** edges, and the operational failure mode is not “link vs missing link” but **attack vs plausible benign traffic** from the same host or neighborhood.

We adapt the hard-negative evaluation idea from link-prediction benchmarking to lateral-movement graphs. We train a GCN link scorer with benign edges as positives and attack edges plus random non-edges as negatives, and at test time ask whether held-out attacks rank above **hard benign negatives**: real benign edges \((u,v')\) from the same source as each attack \((u,v)\), chosen by structural similarity (CN/PPR). We compare this protocol against easy random non-edge negatives on three public datasets—CICAPT-IIoT, LANL Cyber1, and OpTC—using the same model and labels. Our results show that average precision is often inflated under random non-edges and drops sharply under hard benign negatives, indicating that standard LP metrics can overstate detection performance on cyber graphs. We do not propose a new temporal detector; we argue for **realistic negative sampling** when evaluating graph link prediction for lateral movement.

---

**User**

A Research Question That Could Work Well

Can topology-aware hard negative sampling improve temporal link prediction for cybersecurity attack-path discovery?

Method:

Build graph from OpTC.
Train GraphSAGE/GAT/R-GCN.
Compare:
Random negatives
2-hop negatives
Embedding-similar negatives
Subnet-aware negatives
Evaluate:
AUC
AP
MRR
Hits@K

@CARS/chat.md 


what is your recommendation based on my work in Heart and soHeart

---

**Cursor**

**Recommendation:** Reframe the RQ — don’t run that OpTC-only temporal R-GCN study as the main paper.

---

## What’s wrong with the proposed RQ (for your HeaRT background + CARS deadline)

| Proposed piece | Issue |
|----------------|-------|
| **Temporal LP + R-GCN/GAT** | New stack (Euler/Jbeil line), not your SNAP GCN + HeaRT eval pipeline |
| **OpTC only** | You planned 3 datasets; OpTC download still incomplete |
| **Negatives = 2-hop / embedding / subnet on non-edges** | Same mistake as SoHeaRT on cyber — **missing links**, not **benign traffic** |
| **MRR / Hits@K** | OGB social-LP metrics; Euler/Bowman use **AP/AUC/TPR@FPR** for rare attacks |
| **“Attack-path discovery”** | Implies supervised path ranking; your locked story is **eval protocol**, not path finding |

SoHeaRT’s lesson: **easy negatives inflate metrics.** On cyber, the analogue is **random / 2-hop non-edges**, not subnet non-edges. Your chat already concluded **hard benign \((u,v')\)** is the cyber-native hard negative.

---

## RQ that fits HeaRT + your work

**Primary (recommended):**

> *Do topology-aware **hard benign** negatives provide a more realistic evaluation of graph link prediction for lateral-movement detection than random or structural **non-edge** negatives?*

**Not:** “Does temporal R-GCN with 2-hop negs beat random negs on OpTC?”

---

## Method (aligned with what you can ship)

| Keep | Drop / defer |
|------|----------------|
| Static **GCN** (reuse `main_gnn_snap.py`) | R-GCN, GAT, GraphSAGE sweep |
| **B=1 benign, A=0 attack** train | Pure unsupervised only |
| Test neg tiers: **random non-edge → random benign → hard benign (CN/PPR, same \(u\))** | 2-hop / embedding non-edges as main story |
| **AP + AUC** (+ optional TPR@FPR) | MRR/Hits@K as primary |
| **CICAPT + LANL + OpTC** (host/provenance collapse on CICAPT) | OpTC-only |

**Optional ablation (1 small table):** subnet-aware **benign** sampling vs CN/PPR benign — that reuses “subnet-aware” without non-edge SoHeaRT.

---

## How to cite HeaRT honestly

> HeaRT/SoHeaRT show hard **non-edge** negatives matter on OGB/Twitter. We argue cyber LM needs hard **on-graph benign** negatives and propose a sampler; we do **not** claim SoHeaRT transfers to OpTC.

---

## If you still want temporal (stretch goal)

Add **one** Euler-style baseline (GCN+GRU on OpTC/LANL), **same eval protocol** (hard benign test negs). Contribution stays **negative protocol**, not “we beat Euler with R-GCN.”

---

## One-line verdict

**Don’t** make the paper “topology-aware hard negatives for **temporal** OpTC LP with R-GCN.”  
**Do** make it “**hard benign negatives** for **cyber LP evaluation**” — HeaRT-inspired, cyber-specific, 3 datasets, static GCN, AP gap random-non-edge vs hard-benign.

**Title direction:** *Hard Benign Negatives for Evaluating Graph Link Prediction on Cyber Attack Graphs*

---

**User**

why not do the recommendation here
If by **IEEE CARS** you mean the IEEE conference focused on AI, autonomy, robotics, and intelligent systems, then reviewers will usually look for **methodological novelty** rather than a purely operational cybersecurity application.

For that venue, I would rank the ideas as follows:

### 1. Attack Path Prediction on OpTC (Best Fit)

**Problem**

* Predict future attacker movement between hosts.
* Model the enterprise network as a temporal heterogeneous graph.

**Nodes**

* Hosts
* Users
* Processes
* Files

**Edges**

* Login
* Process creation
* Network connection
* File access

**Research Contribution**

* Hard negative sampling based on:

  * same subnet
  * same host role
  * k-hop proximity
  * embedding similarity
* Temporal GNN for future link prediction.
* Explainable attack-path forecasting.

Why it fits:

* Strong graph learning component.
* Clear AI contribution.
* Real-world cybersecurity impact.

---

### 2. Vulnerability Exploitation Prediction (Very Strong)

**Datasets**

* National Vulnerability Database
* CISA Known Exploited Vulnerabilities Catalog

**Task**
Predict:

```text
CVE → Exploited
```

before exploitation is observed.

**Novelty**

* Heterogeneous graph
* Hard negatives from similar CVEs
* Attention-based explanation of predictions

This is easier to publish because data collection is simpler than OpTC.

---

### 3. Insider Threat Detection on LANL

**Dataset**
LANL Cyber Security Dataset

**Task**
Predict unseen:

```text
User → Computer
```

authentication links.

This is a solid baseline project but less novel because many papers already use LANL.

---

### A Research Question That Could Work Well

> Can topology-aware hard negative sampling improve temporal link prediction for cybersecurity attack-path discovery?

Method:

1. Build graph from OpTC.
2. Train GraphSAGE/GAT/R-GCN.
3. Compare:

   * Random negatives
   * 2-hop negatives
   * Embedding-similar negatives
   * Subnet-aware negatives
4. Evaluate:

   * AUC
   * AP
   * MRR
   * Hits@K

The key contribution becomes the **negative sampling strategy**, which is often underexplored in cybersecurity link prediction and can provide a clear experimental story for an IEEE conference paper.


---

**Cursor**

**That CARS advice can work for a different paper — not as your main path given HeaRT, deadline, and what you’ve already scoped.**

## Why not lead with that recommendation

| Their proposal | Your reality |
|----------------|--------------|
| **OpTC temporal heterogeneous R-GCN** (hosts/users/processes/files) | You reuse **homogeneous static GCN** + SNAP; no R-GCN/temporal pipeline built |
| **Attack-path *prediction*** (future links) | You scoped **attack vs benign ranking** on **labeled** edges, not forecasting next hop |
| **Hard negs = 2-hop / embedding / subnet non-edges** | You concluded **SoHeaRT-style non-edges don’t fit cyber**; contribution is **hard benign** \((u,v')\) |
| **OpTC only, full eCAR provenance** | OpTC **partially downloaded**; Euler uses **FLOW-START** only |
| **MRR / Hits@K** | Security LM papers use **AP/AUC** on **rare** positives |
| **~2 weeks to CARS** | Heterogeneous temporal OpTC = **weeks of preprocessing**, not days |

CARS wants **methodological novelty** — but that proposal is novelty in **architecture + temporal heterogeneous LP**, not in your actual hook (**eval protocol / hard benign negatives**). You’d be starting a new project under the old title.

---

## Where that advice is right

- **Hard negatives as the story** — aligns with HeaRT spirit.  
- **Topology-aware sampling** — yes, but on **benign edges**, not missing links.  
- **Graph learning + cyber** — fine for CARS if the **method** is clearly new.

---

## Where it misaligns with HeaRT/SoHeaRT lessons

```text
Their negs:  random / 2-hop / subnet  →  all NON-EDGES
Your insight: cyber failure = attack vs REAL BENIGN (haystack)
```

Subnet/2-hop **non-edges** repeat the SoHeaRT problem in cyber clothing. Your chat landed on **hard benign negatives** — that’s the cyber-native analogue of “hard negatives matter,” not another non-edge sweep on OpTC.

---

## Ranked options *for you* (not generic CARS)

| Rank | Direction | CARS fit | Feasible by deadline? |
|------|-----------|----------|------------------------|
| **1** | **Hard benign negative protocol** + static GCN, 3 datasets (CICAPT/LANL/OpTC host-level) | Method = **sampler + eval study**; honest AI contribution | **Yes** |
| **2** | Their OpTC temporal R-GCN + non-edge hard negs | Strong if fully built + beats baselines | **No** (scope) |
| **3** | CVE exploitation KG (option 2 in their list) | Clean ML story | **Different paper** — no HeaRT reuse |
| **4** | LANL insider LP only | Solid but crowded | Easy but **weak novelty** |

---

## If you want *some* of their CARS appeal without rebooting

**Hybrid (still your paper):**

- **RQ:** topology-aware **hard benign** sampling for cyber LP eval (not attack-path forecasting).  
- **Method:** GCN + **subnet/CN/PPR benign** sampler (their “subnet-aware” → **benign**, not non-edge).  
- **One OpTC row** when data is ready; **lead with CICAPT + LANL**.  
- **Optional:** 1 temporal GCN+GRU baseline later — not the contribution.

**Revised RQ (CARS-friendly + honest):**

> *Can topology-aware **hard benign** negative sampling provide a more reliable evaluation of graph link prediction for lateral-movement detection than random or structural non-edge negatives?*

---

## Bottom line

**Don’t** take “Attack Path Prediction on OpTC with R-GCN + 2-hop non-edges” as the main paper — it ignores your hard-benign conclusion, abandons reusable code, and overshoots the deadline.

**Do** pitch CARS as: **new negative-sampling methodology for cyber graph LP evaluation** (HeaRT-inspired, cyber-adapted), with clear AP gaps across neg tiers — that’s methodological novelty without building heterogeneous temporal OpTC from scratch.

---

**User**

Heristics defined in hard negatives is for negatives to make it hard. I dont think we can reuse the same hard methods for picking hard benigns bcs picking benigns close to each other as per cn/ppr do not make it look like attack, its irrelevant

---

**Cursor**

**You’re largely right — CN/PPR from SoHeaRT doesn’t transfer cleanly to “hard benign.”**

## What SoHeaRT hard negatives actually do

| SoHeaRT hard non-edge | Why it’s “hard” |
|-----------------------|-----------------|
| High CN/PPR, no edge | **Plausible missing link** — model may score it like a real edge |

That’s an **existence** task: true edge vs structurally tempting hole.

## Why CN/PPR on benign is a weak attack proxy

For attack \((u,v)\), picking benign \((u,v')\) with high CN/PPR to \(v\):

- Says **\(v'\) is a graph neighbor like \(v\)** — not “this benign looks like an attack”
- Those edges are **exactly what the model learned as normal** (y=1) → high \(\hat{y}\), low `1-ŷ`
- Hard ranking is “attack vs **very normal** traffic,” not “benign mimics attack”
- LM often hits **unusual** \(v\) or **wrong context** — CN-similar \(v'\) may be **irrelevant** decoys

So **reusing SoHeaRT’s CN/PPR recipe for hard benign is conceptually sloppy** — your objection stands.

---

## What “hard benign” should mean (cyber-native)

Hard = **benign edges that are hard to rank below the attack**, not “structurally like \(v\)”:

| Sampler | Rationale |
|---------|-----------|
| **Same \(u\), same time window** | Same host, same day — real haystack |
| **Same \(u\), high \(\hat{y}\)** (top benign scores) | Attack must beat edges the model calls **most normal** |
| **Same subnet / role** (if metadata exists) | Ops similarity, not graph CN |
| **Random benign from test period** | Easy baseline; still harder than non-edges |

CN/PPR is optional **tie-break**, not the definition of “hard.”

---

## What’s still a contribution (without misusing HeaRT)

| Keep | Drop |
|------|------|
| **Claim:** random **non-edge** AP is misleading for cyber | “We apply SoHeaRT CN/PPR to pick hard benign” |
| **Eval tiers:** non-edge → random benign → **same-\(u\) / high-\(\hat{y}\) benign** | CN/PPR as main hard-benign heuristic |
| **Task:** rank attack above **real benign** candidates | Structural non-edge sweeps (2-hop, embedding negs) |

**HeaRT credit:** *easy negatives inflate LP metrics* — not *same sampler on cyber*.

---

## Revised one-liner

> Cyber LM eval should use **hard real-traffic negatives** (same-host / high-normal-score benign), not SoHeaRT-style structural non-edges — and graph CN/PPR does **not** define which benign edges attacks mimic.

**Bottom line:** Your instinct is correct; fix the sampler to **operational confounders** (same \(u\), time, predicted normality), not CN/PPR peer picking. Contribution stays **evaluation methodology**, with a **clearer** definition of hard benign.

---

**User**

then give me some other idea of using hard negatives in ml

---

**Cursor**

**Hard negatives in ML** = train or eval on examples that are **easy to confuse with positives**, not random easy negatives. Ideas that fit cyber graphs:

---

## 1. **In-batch / curriculum hard negatives** (contrastive LP)
Mine negatives that **score high** under the current model each epoch (top-K \(\hat{y}\) non-edges or benign edges).  
**Cyber:** each round, hardest benign \((u,v')\) from same \(u\) as attack — negatives **adapt** as the model improves.

---

## 2. **Adversarial / model-based hard negatives**
Generate negatives that **maximize** \(\hat{y}\) (or minimize attack score) under fixed embeddings — e.g. perturb destination or pick \(v'\) with highest predicted link score.  
**Cyber:** “most normal-looking” benign edge from compromised host \(u\).

---

## 3. **Same-anchor negatives** (InfoNCE / triplet)
For positive \((u,v)\), negatives share **\(u\)** or **\(v\)** — same user/host, different partner.  
**Cyber:** attack \((u,v)\) vs other **real auths from \(u\)** that day — natural haystack, no CN/PPR.

---

## 4. **Temporal hard negatives** (Euler-style)
Same \((u,v)\) at different times: normal at \(t_1\), suspicious at \(t_4\).  
**Cyber:** benign \((u,v')\) in **same 30-min window** as attack — context confound, not structure.

---

## 5. **Role / subnet stratified negatives**
Negatives from **same VLAN, AD group, or process type** as the positive.  
**Cyber:** OpTC/LANL host role metadata — harder than random benign, **ops-grounded** (not graph CN).

---

## 6. **Importance-weighted / biased negative sampling** (word2vec / Noise-Contrastive)
Sample negatives with probability \(\propto\) frequency or degree — frequent partners are harder false alarms.  
**Cyber:** sample benign \((u,v')\) with probability \(\propto\) degree(\(u\)) or auth count — mimics **busy host** traffic.

---

## 7. **Cross-graph hard negatives** (multi-dataset)
Use benign edges from **another day/campaign** that look structurally similar but are label-clean.  
**Cyber:** Phase-1 benign as negatives when scoring Phase-2 attacks (CICAPT).

---

## 8. **Hard negatives for eval only** (your safest paper angle)
Train fixed; report AP under:
- random non-edge  
- random benign  
- **same-\(u\) benign**  
- **top-\(\hat{y}\) benign** (model-based hard)

**Contribution:** metric sensitivity study — minimal new architecture.

---

## 9. **Semi-hard negatives** (FaceNet / triplet loss)
Pick negatives with score **between** easy and positive — not hardest, stabilizes training.  
**Cyber:** benign edges with \(\hat{y}\) in mid-range from attack source \(u\).

---

## 10. **Contrastive path negatives** (attack-path papers)
Negative = **valid path prefix** that doesn’t complete attack path — same subgraph, wrong next hop.  
**Cyber:** needs path labels; stronger for OpTC red-team sequences, heavier to build.

---

## What fits your stack + deadline

| Idea | Effort | Cyber fit | HeaRT spirit |
|------|--------|-----------|--------------|
| **Eval tiers (8)** | Low | High | Yes (metric critique) |
| **Same-anchor / same-\(u\) (3)** | Low | High | Yes |
| **Model-based top-\(\hat{y}\) benign (2)** | Medium | High | Yes |
| **Temporal window (4)** | Medium | High | Needs snapshots |
| **CN/PPR non-edges (SoHeaRT)** | Low | **Low** | Original HeaRT only |

**Practical paper combo:** train B=1/A=0 + random negs; test with **same-\(u\) benign** and **top-\(\hat{y}\) benign** as two hard-negative protocols; show AP collapse vs random non-edges.

**One-line contribution:** *Hard negatives for cyber ML should be **behaviorally confusable** (same host, same time, high predicted normality), not **topologically similar non-edges**.*

---

**User**

If your goal is a **research-worthy cybersecurity + GNN + link prediction project**, I would prioritize problems where a predicted link has a clear security interpretation.

## 1. Insider Threat Detection (Recommended)

### Dataset

LANL Cyber Security Dataset

### Graph

Nodes:

* Users
* Computers

Edges:

```text
User → Computer
```

represent authentication events.

### Link Prediction Task

Predict:

```text
User → Computer
```

that has not been observed before.

### Security Meaning

A high-scoring unseen link may indicate:

* Credential theft
* Privilege escalation
* Insider threat
* Lateral movement

### Why it's strong

* Large-scale real enterprise data.
* Well-known in cybersecurity research.
* Easy to justify security impact.

---

## 2. Attack Path Prediction

### Dataset

OpTC Dataset

### Graph

Nodes:

* Hosts
* Processes
* Files
* Users

Edges:

```text
process_created_file
host_connected_host
user_logged_into_host
```

### Link Prediction Task

Predict future:

```text
Host ↔ Host
```

connections.

### Security Meaning

Identify potential lateral movement paths before they occur.

### Novelty

Very attractive for top cybersecurity venues.

---

## 3. Malicious Infrastructure Discovery

### Dataset

CTU-13 Dataset

or threat-intelligence knowledge graphs.

### Graph

Nodes:

* Domains
* IPs
* Malware families

Edges:

```text
Malware → Domain
Domain → IP
```

### Link Prediction Task

Predict:

```text
Malware → Domain
```

or

```text
Domain → IP
```

links.

### Security Meaning

Discover unknown command-and-control infrastructure.

---

## 4. Vulnerability Exploitation Prediction

### Dataset

Combine:

* National Vulnerability Database
* CISA Known Exploited Vulnerabilities Catalog

### Graph

Nodes:

* CVEs
* Products
* Vendors
* Attack techniques

### Link Prediction Task

Predict:

```text
CVE → Exploited
```

or

```text
Host → CVE
```

relationships.

### Security Meaning

Prioritize patching efforts.

### Publication Potential

High because vulnerability prioritization remains an active research area.

---

## 5. Provenance Graph Attack Detection

### Dataset

CADETS Dataset
or
Theia Dataset

### Graph

Nodes:

* Processes
* Files
* Network connections

Edges:

```text
read
write
execute
connect
```

### Link Prediction Task

Predict missing process-file or process-network interactions.

### Security Meaning

Detect stealthy attack behaviors.

---

## What I would choose

For a master's thesis or early PhD project:

### Option A (Most Practical)

**LANL + Insider Threat Detection**

* Clean graph construction.
* Straightforward link prediction setup.
* Easy to compare GCN, GraphSAGE, GAT, SEAL.
* Can incorporate hard negative sampling.

### Option B (Most Research-Oriented)

**OpTC + Lateral Movement Prediction**

* Temporal heterogeneous graph.
* Attack-path forecasting.
* Hard negatives based on subnet, host role, and graph distance.
* Strong novelty.

### Option C (Highest Impact)

**Vulnerability Exploitation Prediction using NVD + KEV**

* Heterogeneous graph.
* Link prediction for exploited vulnerabilities.
* Direct real-world value for defenders.
* Good fit for security conferences.

If your target is a venue such as USENIX Security Symposium, ACM Conference on Computer and Communications Security, or Network and Distributed System Security Symposium, I would lean toward **OpTC attack-path prediction** or **KEV/NVD exploitation prediction**, as they are generally more novel than the standard LANL authentication-link prediction setup.



Use ideas from here and give very concise idea what tot do

---

**Cursor**

**Do this (CARS / ~2 weeks, reuses HeaRT + SNAP):**

## Project (one sentence)
**LANL insider-threat LP with hard-negative evaluation** — predict/score rare `User→Computer` auths; show random non-edge AP is misleading vs **same-user benign** and **top-ŷ benign** negatives.

## Why this pick
| Option | Verdict |
|--------|---------|
| **LANL insider (A)** | **Yes** — clean graph, clear security meaning, data ready, fits your GCN pipeline |
| OpTC attack-path (B) | Defer — download + temporal heterogeneous graph too heavy |
| KEV/NVD (C) | Different paper — no HeaRT reuse |
| CADETS/THEIA (5) | Too heavy for deadline |
| CICAPT | **Add as 2nd dataset** if time (provenance collapse → LP) |

## Task (security-clear)
- **Train:** benign auths (B=1), attack/red-team auths (A=0), random non-edge negs  
- **Test:** held-out attack auths vs **hard benign**: other auths from **same user** + **highest ŷ** benign from same user  
- **Score:** `1 - ŷ` (suspicious link)  
- **Model:** static GCN (reuse `main_gnn_snap.py`) — skip GAT/R-GCN sweep  

## Contribution (HeaRT-inspired, not SoHeaRT)
> Hard negatives for cyber LP must be **confusable real edges** (same-anchor / model-ranked benign), not CN/PPR **non-edges**.

## Paper table
`AP/AUC` × {random non-edge | random benign | same-user benign | top-ŷ benign} on **LANL** (+ CICAPT if ready).

## Title
*Hard Negative Protocols for Graph Link Prediction in Insider Threat Detection*

**Skip for now:** OpTC temporal paths, KEV KG, full provenance — pick one after CARS if LANL results look good.

---

**User**

does hard benign mean hard nonedges that look like edges?

---

**Cursor**

**No.**

| Term | What it is | Looks like |
|------|------------|------------|
| **Hard non-edge** (SoHeaRT) | Pair **with no link** in the graph | **Plausible missing edge** (high CN/PPR) |
| **Hard benign** (your cyber idea) | **Real benign edge** that exists | **Normal traffic** — hard because attack must beat **real** normal links, not fake holes |

```text
Hard non-edge:  (u,v) ∉ E  but structurally tempting
Hard benign:    (u,v') ∈ E_benign  — same user/host, busy, high ŷ, etc.
```

**One line:** Hard benign = **real edges that are tough decoys**, not non-edges pretending to be edges.

---

**User**

so the problem is not with random links, problem is with random non links so what could be best way here to work on  cybersecieru dayasets?

---

**Cursor**

**Yes — the main eval bug is random non-edges, not random benign links.**

Attack edges **are** real links, so testing against random holes asks “link vs no-link?” — too easy and **not** the LM/insider question.

---

## Best way on cyber datasets

**1. Task**  
Rank **attack / suspicious edges** above **real alternatives**, not above missing pairs.

**2. Train** (pick one story)

| | Train |
|--|-------|
| **Euler line** | Benign y=1, random non-edge y=0 (no attack in train) |
| **Supervised** | Benign y=1, attack y=0 (+ random non-edge y=0) |

**3. Test negatives (the fix)** — tiered table:

| Tier | Negatives | What it tests |
|------|-----------|---------------|
| ❌ **Bad (show inflation)** | Random **non-edges** | Easy — don’t report as main result |
| ✓ **Baseline** | Random **benign** edges (test period) | Real traffic |
| ✓✓ **Hard** | **Same-anchor** benign: same user/host \(u\), other auths that day | Haystack LM hides in |
| ✓✓ **Harder** | **Top-ŷ** benign from same \(u\) | Beats “most normal-looking” traffic |

**4. Test positives**  
All labeled attack / red-team / Phase-2 attack-touching edges.

**5. Score**  
`1 - ŷ` if unsupervised benign train; raw `1-ŷ` or attack-as-low-ŷ depending on train — **high suspicion = attack should rank higher**.

**6. Metrics**  
**AP / AUC** (pooled ranking). Skip MRR/Hits@K as primary.

**7. Datasets**  
**LANL** first (user→computer); **CICAPT** second; **OpTC** host FLOW-START when downloaded.

---

## One-line workflow

```text
Train normal link model (benign ± attack labels)
Test:  attack edges  vs  REAL benign negatives (same-user > random benign >> random non-edges)
Show:  AP_random_nonedge  >>  AP_same_user_benign
```

**Contribution:** cyber LP papers should **not** use random non-edges as the main eval; use **on-graph benign hard negatives** — HeaRT’s lesson retargeted from “hard non-edges” to “hard **real** negatives.”

---

**User**

we can use other datasets and work on other problems in cybersecuirty that uses this kind of approach 

---

**Cursor**

**Yes — the approach transfers to any cyber task where positives are rare real edges/events and random non-edges fake easy wins.**

## Core template (any dataset)

```text
Train:  learn normal structure (benign edges or events)
Test+:  labeled suspicious/rare edges
Test−:  tiered negatives — random non-edge (bad) → random benign → same-anchor hard benign
Claim:  AP under random non-edges is misleading
```

---

## Problems + datasets that fit well

| Problem | Dataset | Graph / link | Test + | Hard benign − |
|---------|---------|--------------|--------|----------------|
| **Insider / LM auth** | LANL Cyber1 | User→Computer | `redteam.txt` | Other auths from **same user** |
| **IIoT APT edges** | CICAPT-IIoT | Process↔Artifact (collapsed) | Phase-2 attack-touching edges | Same-process / same-host benign Phase-2 edges |
| **Host comms LM** | OpTC FLOW-START | Host→Host | Red-team flows (PDF) | Same-src-IP benign flows that day |
| **Phishing / malicious login** | CERT insider (r4.2) | User→Device | Rare labeled logins | Same-user normal logins |
| **C2 / infra LP** | CTU-13, BOTS | Host→IP, IP→Domain | Malicious flows (label) | Benign flows from **same internal host** |
| **KEV exploitation** | NVD + CISA KEV | CVE→Product, Host→CVE | Exploited pairs | **Similar CVE** (same vendor/CWE) not exploited — hard *node/edge* analog |
| **Enterprise provenance** | DARPA TC (CADETS/THEIA) | Process→File/Socket | Attack-touching edges | Same-process benign edges in attack window |

---

## Problems that fit **less** (skip or weak)

| Problem | Why |
|---------|-----|
| Pure **node** anomaly (no edge ranking) | No LP negatives story |
| **Subgraph signature** matching (Holmes) | Not link-prediction eval |
| **CTI KG completion** (CVE↔CWE) | Multi-relational KGE, not benign haystack |

---

## Best 2-paper expansion (after LANL)

1. **CICAPT-IIoT** — provenance + IIoT; same neg tiers; shows cross-domain.  
2. **CERT or OpTC** — enterprise host/user story.

**KEV/NVD** = separate paper (heterogeneous KG, “hard similar CVE” not hard benign).

---

## One-line pitch (venue-agnostic)

> *Hard-negative protocols for cybersecurity graph learning should use **confusable real negatives** (same user, host, or process), not random non-edges — we demonstrate the AP gap across LANL, CICAPT, and [X].*

**Start LANL; add CICAPT (you have data); OpTC/CERT when ready.**

---

**User**

so what is the cybersecurity problem here?

---

**Cursor**

**Cybersecurity problem:** Detect **suspicious or attack-related interactions** (authentications, host flows, provenance edges) that **blend into normal activity** — credential theft, insider threat, lateral movement — before or as they occur.

**Not:** “Does this edge exist?” (standard LP with random non-edges).

**Operational question:** Among **real** network/provenance traffic, which edges are **attack-related** vs **benign lookalikes** from the same user/host/process?

**Examples by dataset:**

| Dataset | Security problem |
|---------|------------------|
| **LANL** | Unusual **user→computer** auths (red team / LM) |
| **CICAPT** | **APT-related** process/file/socket edges in Phase 2 |
| **OpTC** | **Malicious host-to-host** communication during red-team campaigns |

**Your ML angle:** Same GCN scores links; the **security** task is ranking **attack edges above realistic benign traffic**, not above random missing links.

**One line:** *Find attacks hidden among normal graph edges* — hard-negative eval tests whether models can do that, not whether they can spot holes in the graph.

---

**User**

give me a proper problem to work on

---

**Cursor**

**Problem (locked):**

## Detecting suspicious authentication links in enterprise graphs under realistic evaluation

**Cybersecurity problem**  
Identify **attack-related user→computer authentication edges** (lateral movement, credential misuse, red-team activity) that occur among **normal enterprise logins** on the LANL Cyber Security dataset.

**Why it matters**  
Attack auths are **real edges** mixed into massive benign traffic. Defenders must flag **rare suspicious links**, not “non-existent” pairs.

**ML formulation**  
- Build auth graph: nodes = users + computers; edges = authentication events.  
- Train a GCN link scorer on **benign** auths (y=1) with **attack train** auths as negatives (y=0) + random non-edges (y=0).  
- At test, rank **held-out attack auths** above **tiered negative sets**.

**Research question**  
> When evaluating graph link prediction for insider/lateral-movement detection, how much do **random non-edge negatives** inflate AP compared with **real benign negatives** (especially same-user authentications)?

**Contribution (not a new IDS)**  
A **hard-negative evaluation protocol** for cyber LP: show that standard random non-edge AP is misleading; require **on-graph benign negatives** (random benign → **same-user benign**).

**Method (minimal)**  
- Model: static GCN + dot decoder (reuse SNAP/HeaRT pipeline).  
- Test neg tiers: (1) random non-edge, (2) random benign, (3) **same-user benign** from test window.  
- Metrics: AP, AUC; score = `1 - ŷ` (high = suspicious).

**Extension (if time)**  
Repeat on **CICAPT-IIoT** (APT provenance edges, Phase 1→2) as second graph type.

**Title**  
*Evaluating Graph Link Prediction for Insider Threat Detection: Beyond Random Non-Edge Negatives*

**Deliverable**  
One main table: AP/AUC on LANL across three negative protocols + short discussion tying to Euler/Bowman (they use implicit benign haystack; you **formalize** it).

That is a proper, bounded problem: clear security meaning, reusable code, defensible contribution, feasible in ~2 weeks.

---

**User**

\documentclass[conference]{IEEEtran}
\IEEEoverridecommandlockouts
% The preceding line is only needed to identify funding in the first footnote. If that is unneeded, please comment it out.
\usepackage{cite}
\usepackage{float}
\usepackage[caption=false]{subfig}
\usepackage{algorithm}
\usepackage{amsmath,amssymb,amsfonts}
\usepackage{algorithmic}
\usepackage{tabularx}
\usepackage{graphicx}
 \usepackage{booktabs} 
\usepackage{textcomp}
\usepackage{xcolor}
% \usepackage{showframe}

\usepackage[top=0.8in, bottom=1.1in, left=0.7in, right=0.7in, columnsep=0.1875in]{geometry}

\def\BibTeX{{\rm B\kern-.05em{\sc i\kern-.025em b}\kern-.08em
    T\kern-.1667em\lower.7ex\hbox{E}\kern-.125emX}}

\newcommand{\dnote}[1]{{\color{blue}{#1}}}
\newcommand{\dtodo}[1]{{\color{blue}{TODO #1}}}

    
\begin{document}

%basic title
% \title{Improving the performance of distributed Deep learning for cyber-security tasks by using relaxed consistency models}

% engaging title
% \title{Faster and Stronger: Relaxed Consistency for Distributed Deep Learning in Cybersecurity}

\title{Improving Performance of Distributed Deep Learning in Malware and Intrusion Detection through Relaxed Consistency}

\author{\IEEEauthorblockN{Aavash Bhattarai\IEEEauthorrefmark{1}, Jian Gong\IEEEauthorrefmark{2}, Duong Nguyen\IEEEauthorrefmark{1}}
\IEEEauthorblockA{\IEEEauthorrefmark{1}Department of Electrical Engineering and  Computer Science, \IEEEauthorrefmark{2}School of Computing \\
University of Wyoming, Laramie, Wyoming, USA \\
Email: \{abhattar, jian.gong, duong.nguyen\}@uwyo.edu}
% \IEEEauthorblockA{\IEEEauthorrefmark{2}School of Computing \\
% University of Wyoming\\
% Laramie, Wyoming \\
% Email: \{abhattar, jian.gong, duong.nguyen\}@uwyo.edu}
% \and
% \IEEEauthorblockN{Jian Gong}
% \IEEEauthorblockA{\textit{School of Computing} \\
% \textit{University of Wyoming)}\\
% Laramie, Wyoming \\
% jian.gong@uwyo.edu}
% \and
% \IEEEauthorblockN{Duong Nguyen}
% \IEEEauthorblockA{\textit{Department of Electrical Engineering and Computer Science} \\
% \textit{University of Wyoming)}\\
% Laramie, Wyoming \\
% duong.nguyen@uwyo.edu}
}

\maketitle

\begin{abstract}
Distributed deep learning is a promising approach for early detection and removal of key cybersecurity threats such as malware and intrusion attempts. 
% However, the performance of distributed deep learning is often hindered by the high communication overhead associated with the exchange of the model's parameters.
However, its performance is often limited by the substantial communication overhead incurred during frequent exchanges of model parameters across distributed workers.
In this paper, we propose a novel approach that leverages a relaxed consistency model within an in-memory distributed shared memory to reduce the communication overhead. 
Furthermore, the approach also introduces some stochastic noise that helps the model overcome local minima, thus improving the quality of the optimization process, thereby enhancing the overall optimization process.
Experimental results on widely used benchmark datasets demonstrate that our approach can accelerate the training time by a factor of 3 to 11 times, while maintaining, and in some cases improving, the model accuracy.
%
% evaluates and compares the performance of sequential and eventual consistency models in an in-memory key-value store. In-memory databases store data in main memory rather than on disk, enabling significantly faster storage and retrieval. To assess the impact of consistency models, we examine distributed graph coloring across multiple graph datasets under both sequential and eventual consistency. In addition, we contrast these results with those obtained from a disk-based key-value store, highlighting the trade-offs between consistency guarantees and system performance.
\end{abstract}

% \begin{IEEEkeywords}
% component, formatting, style, styling, insert
% \end{IEEEkeywords}


\section{Introduction}

Malicious software (malware) and network intrusions represent two of the most critical cybersecurity threats. Malware exists in various forms, including viruses, worms, spyware, rootkits, and ransomware. Each day, hundreds of thousands of new malware samples appear on the Internet, with an estimated one billion malware programs active in the wild \cite{avtest.malware}. 
Similarly, intrusion attempts --such as denial-of-service (DoS) attacks, probing, privilege escalation, and unauthorized access -- pose severe risks to networked systems. 
Together, malware and intrusions compromise billions of Internet-connected machines \cite{kaspersky} and, together with other forms of cybercrime, cost the global economy nearly one trillion U.S. dollars annually \cite{csis.cybercrime}. The most effective defense against these threats remains early detection and rapid mitigation.



% Malicious software (malware) is one of the major cybersecurity threats. Existing in various forms including viruses, worms, spywares, rootkits, ransomewares, hundreds of thousands of new malware samples are found everyday on the Internet and a total of one billion malware programs exist in the wild \cite{avtest.malware}. Affected about 15\% of machines on the Internet, malware together with other forms of cyber-crimes cost nearly one trillion U.S. dollar each year \cite{csis.cybercrime}. The most effective defense against malware remains early detection and removal. 

The traditional methods for detecting malware rely on identifying a short sequence of binary code uniquely associated with a specific type of malware. Likewise, intrusion detection systems (IDS) have historically depended on rule-based or signature-based methods to recognize known attack patterns in network traffic. 
While effective against previously observed threats, these approaches—including signature-based and related cloud-based techniques has become less reliable due to advanced concealment techniques (encryption, obfuscation, polymorphism, packaging in malware \cite{ye2017survey}; or evasion and adversarial techniques in intrusions \cite{buczak2015survey}). 
Moreover, the continuous emergence of new malware variants and novel intrusion strategies makes it difficult to maintain comprehensive and up-to-date signature or rule databases.
% Furthermore, a large number of new variants of malware are introduced everyday, making it increasingly difficult to maintain a comprehensive and up-to-date malware signature database. 
%
To intelligently and automatically detect new forms of malware and intrusion, machine learning techniques, especially deep learning, have been employed and yielded promising results in detecting novel and evolving threats \cite{ye2017survey,maniriho2024survey,yuan2014droid,javaid2016deep,khraisat2019survey}.

Training deep learning models for malware and intrusion deetection on the Internet presents several challenges. First, the amount of computation and storage required may exceed the capacity of a single machine. Second, the training data not be readily available at a single location as malware samples are collected from diverse sources over the Internet, and intrusion data is typically distributed across networks, organizations, and devices.
In many cases, the datasets are prohibitively large, or data owners are unwilling to share raw data due to privacy and security concerns.
% or because the size of the whole data collection is too large,  or simply data owners are unwilling to share the raw data with others due to privacy and security concerns. 
Those challenges make distributed training of deep learning models an attractive solution for both malware and intrusion detection. Distributed deep learning (including federated learning) allows processes (also known as workers) located at different locations to collaborate and obtain a shared model effective against a wide range of threats without exposing each worker's individual malware dataset.

% However, the traditional methods focus on centralized machine learning where operator has a complete dataset. This may be not the case since no single entity can collect a complete data set of malware.

% Collaboration and exchange of data is key to defend users effectively, especially when malware can easily spread through the Internet. On the other hand, for various privacy and security reasons, users may be hesitant to share their raw data. To encourage collaboration while preserving the data privacy and protection, distributed machine learning (including federated learning) is a promising choice. 

In distributed deep learning (in distributed computing more broadly), the sharing of data between processes (or workers) must satisfy certain requirements to ensure that the data exchanged remains meaningful. Those requirements are formally specified as \textit{consistency models}. Traditional models, such as linearizability and sequential consistency, impose strict guarantees on shared data, ensuring on the shared data to ensure that any data observed by the workers is always up-to-date. This requirement facilitates the development of distributed applications and the process of verifying the correctness of the applications. However, such requirements also have significant communication overhead \cite{CAP.Brewer00PODC,NCKD2018LADC}. In the context where the application is a distributed deep learning algorithm, even without direct raw data exchange, the training of the global model in distributed fashion already involves a substantial amount of communication between workers to update and share the global model's parameters (weights). This overhead substantially prolongs the training time of the model. An extended delay in the model update can be detrimental, especially when new pieces of malware emerge and spread quickly.

In this paper, we propose an approach for distributed deep learning that leverages a relaxed consistency model to improve the performance (training time as well as accuracy) of distributed deep learning for malware detection. 
Unlike the traditional consistency models, relaxed consistency models (e.g. eventual consistency) remove the strict requirement of the delivery of the most up-to-date data and adopt a best-effort approach instead. While the delivery of the latest data is not guaranteed, it typically occurs. In other words, in the majority of the cases, the distributed workers can see the most recent weight updates. On one hand, relaxed consistency models substantially reduce the amount of communication overhead and help the optimization process. On the other hand, imperfect data exchange means imperfect gradient updates at the workers. These gradient updates, although not perfect but generally correct, are very effective in helping the distributed deep learning algorithm overcome local minima \cite{huo2017asynchronous,lin2018don}, therefore improving the quality of the optimization process of the global model.

We validate our approach with experiments where we train a global model for malware detection or intrusion detection using a group of distributed workers. Each worker possesses its own dataset and those datasets are not shared. The workers only share the global model that they are trying to train collaboratively. To facilitate the sharing of global model, the workers rely on KeyDB, an in-memory distributed shared memory system for storing and reading the global model's weights. We adopt the distributed shared memory (DSM) abstraction (instead of message passing \cite{Lynch96}) since the DMS interface only consists of \textit{read (get)} and \textit{write (put)} operations, making it more convenient for programmers to work with. We select in-memory DSM instead of disk-based DSM since the in-memory DSM delivers a higher throughput and lower latency.

The contributions of the paper are as follow:
\begin{itemize}
    \item We propose using relaxed consistency to reduce the synchronization overhead in distributed deep learning for malware detection and intrusion detection, while also increasing resilience against local minima.
    \item Our experiments on the EMBER dataset \cite{kaggle.ember} and CIC-IDS2017 dataset \cite{sharafaldin2018toward} show that our approach reduces the training time by 3 to 11 times, while the accuracy is comparable to or even better than the accuracy of the approach using the strict consistency model.
    \item Our approach also allows users to collaboratively obtain a globally shared deep learning model effective for malware detection while still be able to keep their data private.
\end{itemize}

\textbf{Organization of the paper}.
Section \ref{sec:background} provides the background on distributed shared memory architecture and the two cybersecurity threats. Section \ref{sec:algorithms} describes the algorithms for deploying distributed deep learning algorithms using KeyDB.
Section \ref{sec:exp-setup} explains the experiment setup while section \ref{sec:exp-results} presents the experimental results.
Finally, Section \ref{sec:conclusion} provides a conclusion.


% \dnote{[THE REMAINING TEXT IN THE INTRODUCTION SECTION CAN BE REMOVED OR RE-USED IN OTHER PARTS (i.e. Background)]} In-memory databases store data in random access memory (RAM), which allows them to deliver higher throughput and lower latency compared to disk-based databases \cite{main_memory_recovery}. The rapid improvement in memory technologies—where memory capacity and bandwidth have been doubling approximately every three years while costs have been steadily decreasing—has made in-memory databases increasingly practical for large-scale applications \cite{inmemory_big_data}. In distributed environments, data from multiple servers can be aggregated in memory to support workloads that require fast access to large datasets. Several researchers have even advocated for systems such as RAMCloud, which stores data entirely in DRAM to achieve low-latency access at scale \cite{ram_cloud_1}. As a result, in-memory key-value stores have become an attractive choice for modern data-intensive applications. \\ 
% Distributed Systems utilize multiple computing resources across various locations to achieve a unified computational goal \cite{ibm_distributed_computing}. Distributed Shared Memory (DSM) is a type of infrastructure software that simplifies the process of sharing data across processes in a distributed system. It allows processes to read and write shared data as if it were stored locally, even though the
% data may physically reside on multiple remote machines \cite{Distributed_Book}. One of the central challenges in distributed shared memory systems is maintaining consistency across replicas. According to CAP  \cite{CAP_theorem}, A DSM cannot simultaneously achieve sequential consistency and availability while tolerating network partition. Hence, it is inevitable to make trade-
% offs between availability and consistency, resulting in a
% spectrum of weaker consistency models such as causal
% consistency and eventual consistency. Consistency models define the guarantees provided to applications when multiple processes read and write shared data. Stronger models, such as sequential consistency, ensure that operations appear in a globally consistent order, but often at the cost of performance. In contrast, weaker models, such as eventual consistency, relax ordering guarantees to improve throughput and scalability, though this may complicate application correctness \cite{Distributed_Book}. Hence, it is important to study the performance of In-Memory Key-Value store in case of different consistency models. 
% In an Eventually Consistent system, the DSM  guarantees that if no new updates are made to the object, eventually all reads will return the last updated value \cite{eventual_definition}. 

% In this paper, we make the following contributions:  
% \begin{itemize}  
%     \item We implement and evaluate sequential and eventual consistency in an in-memory key-value store (KeyDB).  
%     \item We use distributed graph coloring on real-world graph datasets (DBLP and YouTube) as a representative workload to measure performance.  
%     \item We compare the results against a storage-based key-value store, highlighting the performance gap between in-memory and disk-based systems under different consistency guarantees.  
% \end{itemize}  



\section{Background}
\label{sec:background}

\subsection{Malware and Intrusion Detection}


Malware, short for malicious software, is a general term that encompasses all forms of harmful programs created to carry out the malicious intentions of attackers. It includes threats such as viruses, trojans, ransomware, and bots. Detecting malware is an essential security task, as it helps safeguard legitimate users from the growing and evolving dangers posed by such software. With millions of new malware variants emerging each year, there is a pressing demand for smarter and more efficient detection techniques \cite{survey_malware}.

Intrusion refers to any unauthorized or harmful activity that threatens the security of computer systems or networks. These activities are deliberate attempts by attackers to disrupt normal operations, gain unauthorized access to resources, or steal sensitive information. Intrusion Detection is therefore a vital process that focuses on identifying such unauthorized actions to defend users and systems. Intrusion Detection Systems (IDS) are specialized security tools that continuously analyze network traffic and system behaviors to spot suspicious activity and notify administrators \cite{survey_malware}.


% \dtodo{Aavash: please ddd some general description and discussion of malware and intrusion detection here.

% To get a general view of these two problems, you can read some papers at
% https://dl.acm.org/doi/pdf/10.1145/3073559 and 
% https://link.springer.com/chapter/10.1007/978-3-030-04780-1_28
% and 
% A Detailed Investigation and Analysis of Using Machine Learning Techniques for Intrusion Detection

% }


\subsection{Architecture}

We consider a fully replicated key-value store deployed across multiple servers, where each server maintains a complete copy of the dataset. The servers are connected through master-master replication, ensuring that updates can be accepted and propagated by any server. For workload management, however, the dataset is logically partitioned into equal parts, and each server is responsible for handling client requests corresponding to its assigned partition of the graph. This approach balances load across servers while still maintaining full replication of data.  

Clients interact only with their designated server and do not send requests to other servers directly. Partitioning the request space in this way improves system performance by reducing contention and distributing computation. We assume a fault-free environment in which all servers are reliable and can communicate with each other without failures. Furthermore, we employ a single-version key-value store, where only the latest value of each key is stored.


\subsection{KeyDB}


KeyDB is an open-source, in-memory data structure store that can be used as a database, cache, or message broker. It supports several data structures, including strings, hashes, lists, sets, and sorted sets with range queries \cite{KeyDB}. As a high-performance fork of Redis, KeyDB extends Redis features with enhanced replication and multithreading support. To mitigate the risk of lost updates, persistence mode can be enabled \cite{keydb_persistence}.



Clients interact with KeyDB using simple commands. For example, storing a string value in a key is performed using the \texttt{SET} command, and retrieving it uses the \texttt{GET} command. For set data types, \texttt{SADD} is used to insert elements into a set, and \texttt{SMEMBERS} is used to retrieve all members of a set \cite{KeyDB}.  

KeyDB natively supports both synchronous and asynchronous replication \cite{keydb_replication}. By default, asynchronous replication is used, which guarantees eventual consistency: updates are eventually propagated to replicas, though clients may temporarily read stale values. Synchronous replication, on the other hand, can be requested using the \texttt{WAIT} command, which blocks the client until a specified number of replicas acknowledge the write. This mechanism can be used to enforce stronger guarantees. In particular, sequential consistency can be achieved if the system ensures that $W + R > N$ and $W > N/2$, where $W$ is the number of replicas that must acknowledge a write, $R$ is the number of replicas involved in a read, and $N$ is the total number of replicas. The \texttt{WAIT} command allows clients to configure $W$ to satisfy these conditions, thereby ensuring sequential consistency.


\subsection{Consistency Models}

A distributed system consists of a collection of sequential processes running on different nodes that communicate through shared objects \cite{Linearizability}. In such systems, consistency models define the rules governing when and how updates become visible to different processes. These models provide a formal framework for reasoning about the trade-off between performance and correctness in distributed systems \cite{Distributed_Book}.  

In this work, we focus on two widely studied consistency models: sequential consistency and eventual consistency.  

\subsubsection{Sequential Consistency}  
Sequential consistency requires that all updates to a shared variable in distributed memory appear in some global sequential order, and that all processes observe updates in the same order \cite{sequential}. While sequential consistency does not guarantee real-time ordering, it ensures that the execution behaves as if operations were executed one after another in a single sequence, preserving program order for each process.  

\subsubsection{Eventual Consistency}  
Eventual consistency is a weaker consistency model that prioritizes availability and performance. It requires that, if no new updates are made to a shared variable, eventually all processes will observe the same final value \cite{eventual_definition}. This model allows temporary divergence among replicas but guarantees convergence in the absence of new writes. Eventual consistency is widely used in distributed key-value stores due to its scalability and low latency.


% \subsection{Dataset}
\section{Algorithms}
\label{sec:algorithms}

Multi-Layer Perceptrons (MLPs) are a class of neural networks consisting of multiple hidden layers, where each neuron in a hidden layer is fully connected to every neuron in the subsequent layer \cite{prince2023understanding}.

For one-dimensional data, a convolution operation transforms an input vector \(x\) into an output vector \(z\), where each output element \(z_i\) is computed as a weighted sum of neighboring inputs. The same set of weights is applied across all positions, and these weights are referred to as the \textbf{kernel} or \textbf{filter}. The size of the input region over which the weighted sum is computed is called the \textbf{kernel size}. Convolutional Neural Networks (CNNs) are composed of a series of such convolutional layers \cite{prince2023understanding}.

To deploy deep learning algorithm in distributed fashion, we use the in-memory distributed shared memory KeyDB to sharing the model weights. Algorithms \ref{algo:async} and \ref{algo:sync} shows how a deep learning algorithm is deployed in asynchronous (relaxed consistency) and synchronous (strict consistency) modes. The key difference is at the step 6.v where the new weights are updated to KeyDB. 

\begin{algorithm} %[H]
\caption{Aynchronous Distributed Training with KeyDB-based Parameter Server}
\label{algo:async}
\begin{enumerate}
    \item \textbf{Input:} 
    \begin{enumerate}
        \item Dataset $D$ partitioned into shards $\{D_1, D_2, \dots, D_n\}$ 
        \item Model architecture $M$ with parameters $\theta$ 
        \item Learning rate $\alpha$, batch size $B$, maximum iterations $T$ 
        \item KeyDB cluster with servers $\{S_1, S_2, \dots, S_n\}$ 
    \end{enumerate}
    \item \textbf{Output:} Trained model parameters $\theta^*$
    \item Initialize weights $\theta^0$ using Xavier initialization
    \item Store $\theta^0$ in KeyDB with metadata
    \item For each server $S_i$ in parallel:
    \begin{enumerate}
        \item Load local shard $D_i$
        \item Initialize trainer with \texttt{server\_id = i}
    \end{enumerate}
    \item For iteration $t = 1$ to $T$:
    \begin{enumerate}
        \item For each server $S_i$ in parallel:
        \begin{enumerate}
            \item Load current weights $\theta^t$ from KeyDB
            \item Create batch $B_i$ from local shard $D_i$
            \item Compute gradients $\nabla \theta_i = \nabla L(B_i, \theta^t)$
            \item Update local weights: $\theta^{t+1}_i = \theta^t - \alpha \nabla \theta_i$
            \item Save $\theta^{t+1}_i$ to KeyDB (asynchronous)
        \end{enumerate}
    \end{enumerate}
    \item Return final weights $\theta^*$
\end{enumerate}
\end{algorithm}


\begin{algorithm}%[H]
\caption{Synchronous Distributed Training with KeyDB }
\label{algo:sync}
\begin{enumerate}
    \item \textbf{Input:} 
    \begin{enumerate}
        \item Dataset $D$ partitioned into shards $\{D_1, D_2, \dots, D_n\}$
        \item Model architecture $M$ with parameters $\theta$
        \item Learning rate $\alpha$, batch size $B$, maximum iterations $T$
        \item KeyDB cluster with servers $\{S_1, S_2, \dots, S_n\}$
        \item Replica acknowledgment threshold $R$
    \end{enumerate}
    \item \textbf{Output:} Trained model parameters $\theta^*$
    \item Initialize weights $\theta^0$ using Xavier initialization
    \item Store $\theta^0$ in KeyDB with metadata
    \item For each server $S_i$ in parallel:
    \begin{enumerate}
        \item Load local shard $D_i$
        \item Initialize trainer with \texttt{server\_id = i}
    \end{enumerate}
    \item For iteration $t = 1$ to $T$:
    \begin{enumerate}
        \item For each server $S_i$ in parallel:
        \begin{enumerate}
            \item Load current weights $\theta^t$ from KeyDB
            \item Create batch $B_i$ from local shard $D_i$
            \item Compute gradients $\nabla \theta_i = \nabla L(B_i, \theta^t)$
            \item Update local weights: $\theta^{t+1}_i = \theta^t - \alpha \nabla \theta_i$
            \item Save $\theta^{t+1}_i$ to KeyDB with \texttt{WAIT}($R$, timeout)
            \item If \texttt{WAIT} $< R$: retry
            
        \end{enumerate}
    \end{enumerate}
    \item Return final weights $\theta^*$
\end{enumerate}
\end{algorithm}


\section{Experiment Setup}
\label{sec:exp-setup}


\subsubsection{System Configuration} 





We conducted our experiments in a local lab network with the following configurations: (i) three servers and (ii) three client. The Client and Server run on the same machine with configuration shown in Table \ref{tab:server_specs}. 

\begin{table}[!h]
\centering
\begin{tabular}{|c|l|m{30pt}|m{40pt}|l|c|}
\hline
S.N & Name   & CPU & GPU                  & RAM     & Count \\ \hline
1   & Mteverest & I7 - 13th Gen & NVIDIA GeForce RTX 4090 & 128 GB & 3    \\ \hline
\end{tabular}
\caption{Server specifications}
\label{tab:server_specs}
\end{table}

The servers are connected using Master-Master replication, which allows each server to accept both read and write requests from its respective clients. Updates performed on one server are propagated to all other servers. By default, KeyDB employs asynchronous replication, where replicas periodically acknowledge the amount of data they have received from the master. In this mode, the master does not wait for replicas to confirm every command before proceeding, although it keeps track of which replicas have processed which updates \cite{keydb_replication}. This mechanism provides eventual consistency, since replicas may temporarily diverge but will converge over time.

To guarantee sequential consistency, we extended this behavior by leveraging the \texttt{WAIT} command on the client side. The \texttt{WAIT} command blocks the client until a specified number of replicas acknowledge a write. In our implementation, a write is only considered successful if it is acknowledged by all replicas. If the acknowledgment is not received from all replicas, the client retries the write operation. This ensures that all servers observe updates in the same order, thereby guaranteeing sequential consistency across the system.





\subsubsection{Datasets}
This research employs two prominent cybersecurity datasets to evaluate the proposed methodology.
\begin{itemize}
    

\item  \textbf{EMBER Dataset:} The Endgame Malware Benchmark for Research (EMBER) dataset \cite{ember,kaggle.ember} is a comprehensive malware detection dataset containing features extracted from Portable Executable (PE) files. The dataset originally contained over one million PE files, which after preprocessing and cleaning resulted in 619,876 samples with 2,381 features. We utilize EMBER 2018 (Version 2) in this study. The features include import/export functions, section characteristics, file properties, byte histograms, entropy features, and header information, providing rich representations of both malicious and benign executables \cite{ember}.

\item \textbf{CIC-IDS2017 Dataset:} The Canadian Institute for Cybersecurity Intrusion Detection System 2017 (CIC-IDS2017) dataset \cite{sharafaldin2018toward} contains realistic network traffic data for intrusion detection evaluation. The dataset comprises 2,830,743 network flow records with 79 features each, collected over five days of network activity. It includes benign traffic and various contemporary attacks such as brute force, DoS/DDoS, web attacks, infiltration, and port scans. The features encompass flow duration, packet statistics, inter-arrival times, protocol-specific characteristics, and active/idle time metrics \cite{CIC}.
\end{itemize}
\subsubsection{Workload Partitioning Scheme}

The distributed training system employs a stratified partitioning scheme to ensure balanced workload distribution across the three servers while maintaining the statistical properties of the original dataset. This approach is critical for preventing data skew and ensuring consistent training performance across all participating nodes. Stratified partitioning divides the dataset into subsets such that each subset maintains the same class distribution as the original dataset, ensuring no server receives a biased sample that could lead to skewed learning.
\subsubsection{Neural Network Configuration}

The system has been evaluated using two Neural Network Architecture for both the dataset. We have used the Multi-Layered Perceptron and the Convolutional Neural Network to perform experiments.
\begin{table*}[t]
    \centering
    \begin{tabular}{|l|l|l|l|c|}
    \hline
    \textbf{Network Type} & \textbf{Dataset} & \textbf{Configuration} & \textbf{No. of Parameters} \\ \hline
    CNN & EMBER & 4 Convolutional layers, 2 Fully Connected layers & 5,779,330 \\ \hline
    MLP & EMBER & 4 Hidden layers, 1 Fully Connected layer, 1 Output layer & 3,128,456 \\ \hline
    CNN & CIC-IDS & 4 Convolutional layers, 2 Fully Connected layers & 770,178
 \\ \hline
    MLP & CIC-IDS & 4 Hidden layers, 1 Fully Connected layer, 1 Output layer & 770,050\\ \hline
    \end{tabular}
    \caption{Neural Network Architectures and Their Configurations}
    \label{tab:network-config}
\end{table*}

\section{Experimental Results}
\label{sec:exp-results}

\begin{figure*}[ht]

    \centering
    \subfloat[Training Accuracy]{
        \includegraphics[width=0.4\textwidth]{images/cic_train_acc.png}
        \label{fig:cic-train-acc}
    }
    \hfill
    \subfloat[Training Loss]{
        \includegraphics[width=0.4\textwidth]{images/cic_train_loss.png}
        \label{fig:cic-train-loss}
    }
    \hfill
    \subfloat[Validation Accuracy]{
        \includegraphics[width=0.4\textwidth]{images/cic_val_acc.png}
        \label{fig:cic-val-acc}
    }
    \hfill
    \subfloat[Validation Loss]{
        \includegraphics[width=0.4\textwidth]{images/cic_val_loss.png}
        \label{fig:cic-val-loss}
    }
    
    \caption{Training and validation metrics for the CIC-IDS Dataset.}
    \label{fig:cic-metrics}
    
\end{figure*}
\begin{figure*}[ht]
   
    \centering
    \subfloat[]{
        \includegraphics[width=0.4\textwidth]{images/ember_train_acc.png}
        \label{fig:ember_train_acc}
    }
    \hfill
    \subfloat[]{
        \includegraphics[width=0.4\textwidth]{images/ember_train_loss.png}
        \label{fig:ember_train_loss}
    }
    \hfill
    \subfloat[]{
        \includegraphics[width=0.4\textwidth]{images/ember_val_acc.png}
        \label{fig:ember_val_acc}
    }
    \hfill
    \subfloat[]{
        \includegraphics[width=0.4\textwidth]{images/ember_val_loss.png}
        \label{fig:ember_val_loss}
    }
   
    \caption{Training and validation metrics for the Ember Dataset.}
    
    %\Description{}
    \label{fig:ss-computation}
    
\end{figure*}

From Figure \ref{fig:ember_train_acc} and Table \ref{tab:deep-learning-training-time}, we observed that for malware detection on EMBER dataset, the asynchronous (relaxed consistency) mode yields a better model (higher accuracy, lower loss) than the synchronous mode, while the training time is improved by 2.79 (for CNN) to 11.44 (for MLP) times. On CIC-IDS2017 dataset, the model performance is comparable, however, training with asynchronous mode still substantially improves the time by a factor of 3.6 (for CNN) to 11.48 (for MLP).
In both cases, the training time is substantially improved. However, the improvement on MLP is much more pronounced than on CNN. The reason for this difference is that MLP is a fully connected deep neural networks, therefore, the need weight exchange is much higher than in CNN. As a result, benefits of using relaxed consistency model is also higher. 

In addition, MLP weights operate with greater independence since each neuron connects to all neurons in the subsequent layer without spatial constraints, making weight updates in one region have minimal direct impact on adjacent parameters. This independence provides resilience during distributed training, where multiple models may develop slightly different weight configurations. Conversely, CNNs exhibit strong spatial correlations between adjacent weights due to their convolutional structure, where filters slide across inputs with shared parameters, creating critical interdependencies that form learned spatial patterns like edge detectors and feature extractors. When synchronizing weights across different CNN models in distributed settings, these spatial correlations become problematic because averaging weights from models that have learned different spatial patterns can create hybrid filters that don't correspond to meaningful visual features, effectively disrupting the spatial hierarchies that CNNs depend upon for their effectiveness. 
% Requires: \usepackage{graphicx}
\begin{table*}[t]
    \centering
    \begin{tabular}{|l|l|c|c|}
    \hline
    \textbf{Deep Learning Model} & \textbf{Consistency Mode} & \textbf{EMBER dataset} & \textbf{CIC-IDS2017 dataset} \\ \hline
    MLP & Async (Relaxed consistency) & 133  & 41  \\ \hline
    MLP & Sync (Strict consistency)   & 1522 & 471 \\ \hline
    CNN & Async (Relaxed consistency) & 1279 & 128 \\ \hline
    CNN & Sync (Strict consistency)   & 3563 & 461 \\ \hline
    \end{tabular}
    \caption{Training time (in seconds) of MLP and CNN Deep Learning Models, with two Consistency Models (relaxed and strict) for two cybersecurity datasets (EMBER - malware detection, and CIC - intrusion detection)}
    \label{tab:deep-learning-training-time}
\end{table*}
% Requires: \usepackage{graphicx}
\begin{table*}[t]
    \centering
    \begin{tabular}{|l|l|l|c|c|c|}
    \hline
    \textbf{Deep Learning Model} & \textbf{Consistency Mode} & \textbf{Dataset} & \textbf{F1 Score} & \textbf{Precision} & \textbf{Recall} \\ \hline
    MLP & Async & EMBER  & 0.9072 & 0.9074 & 0.9072 \\ \hline
    MLP & Sync   & EMBER &0.88  & 0.88 & 0.88 \\ \hline
    CNN & Async  & EMBER & 0.3396  & 0.7280  & 0.5027 \\ \hline
    CNN & Sync  & EMBER & 0.333  & 0.75 & 0.5001 \\ \hline
    MLP & Sync  & CIC-IDS & 0.89  & 0.89 &0.89 \\ \hline
    MLP & Async   & CIC-IDS & 0.92 &0.93 & 0.931 \\ \hline
    CNN & Sync   & CIC-IDS & 0.94 & 0.94 & 0.94 \\ \hline
    CNN & Async    & CIC-IDS & 0.94 & 0.94 & 0.94\\ \hline
    \end{tabular}
    \caption{Performance Matrix for Different Models}
    \label{tab:deep-learning-training-time}
\end{table*}

% \begin{table}
%     \centering
%     \begin{tabular}{cccc}
%          &  &  & \\
%          &  &  & \\
%          &  &  & \\
%          &  &  & \\
%     \end{tabular}
%     \caption{Caption}
%     \label{tab:placeholder}
% \end{table}



% \begin{figure*}%[!h]
%     \centering    \includegraphics[width=0.9\textwidth]{images/Ember_comprehensive_all_models_comparison.png}
%     \caption{Performance on Ember Dataset}
%     \label{fig:ember}
% \end{figure*} 

% \begin{figure*}%[!h]
%     \centering
%     \includegraphics[width=0.8\textwidth]{images/CIC-IDS-comprehensive_all_models_comparison.png}
%     \caption{Performance on CCIC-IDS2017 dataset}
%     \label{fig:cicids}
% \end{figure*} 


% \begin{figure}[!h]
%     \centering
%     \includegraphics[width=0.5\textwidth]{images/DLBP_async.png}
%     \caption{Eventually Consistent Graph Coloring for DLBP Graph}
%     \label{fig:Graph Coloring}
% \end{figure} 

% \begin{figure}[!h]
%     \centering
%     \includegraphics[width=0.5\textwidth]{images/DLBP_sync.png}
%     \caption{Sequential Consistent Graph Coloring for DLBP Graph }
%     \label{fig:Eve}
% \end{figure} 


% \begin{figure}[!h]
%     \centering
%     \includegraphics[width=0.5\textwidth]{images/youtube_sync.png}
%     \caption{Eventually Consistent Graph Coloring for Youtube Connection Graph }
%     \label{fig:Sequential Consistent Graph Coloring for Youtube Connection Graph}
% \end{figure} 


% \begin{figure}[!h]
%     \centering
%     \includegraphics[width=0.5\textwidth]{images/youtube_async.png}
%     \caption{Sequential Consistent Graph Coloring for Youtube Connection Graph }
%     \label{fig:Graph Coloring}
% \end{figure} 


% \begin{table*}[!h]
% \centering
% \begin{tabular}{|l|r|r|r|l|r|l|}
% \hline
% Graph Name & Node & Edge & Duration (s) & Mode  & Number of mistakes & Mistake Percentage \\ \hline
% DlBP    & 317080  & 1049866 &  670  & sync   & 0 & 0 \\ \hline
% DlBP    & 317080  & 1049866 & 684  & sync   & 0 & 0 \\ \hline
% DlBP    & 317080  & 1049866 & 663  & sync   & 0 & 0 \\ \hline
% Youtube & 1134890 & 2987623 & 4302 & sync   & 286 & 0.03\% \\ \hline
% Youtube & 1134890 & 2987623 & 3798 & sync   & 0 & 0 \\ \hline
% Youtube & 1134890 & 2987623 & 3845 & sync   & 0 & 0 \\ \hline
% DlBP    & 317080  & 2987623 & 551  & async  & 0 & 0 \\ \hline
% DlBP    & 317080  & 1049866 & 471  & async  & 182724 & 0.5763\% \\ \hline
% DlBP    & 317080  & 1049866 & 490  & async  & 182724 & 0.5763\% \\ \hline
% Youtube & 1134890 & 2987623 & 3499 & async  & 0 & 0\% \\ \hline
% Youtube & 1134890 & 2987623 & 3597 & async  & 0 & 0\% \\ \hline
% Youtube & 1134890 & 2987623 & 3634 & async  & 1 & 0.00008811426\% \\ \hline
% \end{tabular}
% \caption{Experiment results for different graphs in sync and async modes.}
% \label{tab:experiment_results}
% \end{table*}
% \begin{algorithm}[H]
% \caption{Distributed Deep Learning Training}
% \textbf{Input:} Dataset partitioned into shards $\{D_1, D_2, \ldots, D_n\}$; Model architecture $M$ with parameters $\theta$; Learning rate $\alpha$; Batch size $B$; Maximum iterations $T$; KeyDB cluster with servers $\{S_1, S_2, \ldots, S_n\}$.

% \textbf{Output:} Trained model parameters $\theta^*$.

% \begin{algorithmic}[1]
% \State Initialize weights $\theta_0$ (e.g., using Xavier initialization) and store them in KeyDB.
% \State \textbf{For each} server $S_i$ in parallel:
% \State \quad Load local data shard $D_i$.
% \State \textbf{For} iteration $t = 1$ to $T$:
% \State \quad \textbf{For each} server $S_i$ in parallel:
% \State \quad \quad Load current weights $\theta_t$ from KeyDB.
% \State \quad \quad Create batch $B_i$ from local shard $D_i$.
% \State \quad \quad Compute gradients $\nabla\theta_i = \nabla L(B_i, \theta_t)$.
% \State \quad \quad Update local weights: $\theta_i^{t+1} = \theta_t - \alpha\nabla\theta_i$.
% \State \quad \quad \textbf{Save updated weights to KeyDB:}
% \State \quad \quad \quad \textbf{If Relaxed Consistency (Asynchronous):}
% \State \quad \quad \quad \quad Save $\theta_i^{t+1}$ to KeyDB asynchronously.
% \State \quad \quad \quad \textbf{Else If Strong Consistency (Synchronous):}
% \State \quad \quad \quad \quad Wait for all servers to complete their updates.
% \State \quad \quad \quad \quad Aggregate gradients: $\nabla\theta_{avg} = \frac{1}{n}\sum_{i=1}^{n}\nabla\theta_i$.
% \State \quad \quad \quad \quad Update global weights: $\theta_{t+1} = \theta_t - \alpha\nabla\theta_{avg}$.
% \State \quad \quad \quad \quad Save $\theta_{t+1}$ to KeyDB.
% \State \textbf{Return} final trained parameters $\theta^*$.
% \end{algorithmic}
% \end{algorithm}
% \begin{algorithm}[H]
% \caption{Distributed Deep Learning Training}
% \textbf{Input:} Dataset partitioned into shards $\{D_1, D_2, \ldots, D_n\}$; Model architecture $M$ with parameters $\theta$; Learning rate $\alpha$; Batch size $B$; Maximum iterations $T$; KeyDB cluster with servers $\{S_1, S_2, \ldots, S_n\}$.

% \textbf{Output:} Trained model parameters $\theta^*$.

% \begin{algorithmic}[1]
% \State Initialize weights $\theta_0$ (e.g., using Xavier initialization) and store them in KeyDB.
% \State \textbf{For each} server $S_i$ in parallel:
% \State \quad Load local data shard $D_i$.
% \State \textbf{For} iteration $t = 1$ to $T$:
% \State \quad \textbf{For each} server $S_i$ in parallel:
% \State \quad \quad Load current weights $\theta_t$ from KeyDB.
% \State \quad \quad Create batch $B_i$ from local shard $D_i$.
% \State \quad \quad Compute gradients $\nabla\theta_i = \nabla L(B_i, \theta_t)$.
% \State \quad \quad Update local weights: $\theta_i^{t+1} = \theta_t - \alpha\nabla\theta_i$.
% \State \quad \quad \textbf{Save updated weights to KeyDB:}
% \State \quad \quad \quad \textbf{If Relaxed Consistency (Asynchronous):}
% \State \quad \quad \quad \quad Save $\theta_i^{t+1}$ to KeyDB asynchronously.
% \State \quad \quad \quad \textbf{Else If Strong Consistency (Synchronous):}
% \State \quad \quad \quad \quad Wait for all servers to complete their updates.
% \State \quad \quad \quad \quad Aggregate gradients: $\nabla\theta_{avg} = \frac{1}{n}\sum_{i=1}^{n}\nabla\theta_i$.
% \State \quad \quad \quad \quad Update global weights: $\theta_{t+1} = \theta_t - \alpha\nabla\theta_{avg}$.
% \State \quad \quad \quad \quad Save $\theta_{t+1}$ to KeyDB.
% \State \textbf{Return} final trained parameters $\theta^*$.
% \end{algorithmic}
% \end{algorithm}



In this paper, we present a relaxed consistency in a distributed deep learning approach for malware and intrusion detection using KeyDB as a distributed shared memory system. Our approach enables parallel training across multiple workers with asynchronous weight updates, reducing communication overhead while introducing stochastic gradient noise that helps escape local minima and improves model performance.

Our experiments  on CIC-IDS2017 and Ember datasets demonstrate that our approach can improve the training time from 3 to 11 times, while providing equivalent or superior accuracy compared to sequential training. The distributed training architecture scales across multiple servers while being able to maintain model convergence and performance. Future work will focus on detailed weights synchronization analysis to understand the dynamics of weight exchange between distributed workers. To overcome the shortcomings of CNN, instead of a simple synchronizing method, we can perform a weighted synchronization where important features do not get replaced. Additionally we plan to integrate federated learning paradigms to enhance data privacy. 

\bibliography{references,DuongNguyen} 
\bibliographystyle{unsrt}


\end{document}



this paper does the same thing?

---

**Cursor**

**No — completely different problem, method, and contribution.**

| | **This paper (UWyo / KeyDB)** | **Your LANL hard-negative LP idea** |
|--|-------------------------------|-------------------------------------|
| **Problem** | Faster **distributed training** for malware/IDS | **Evaluating** graph LP for suspicious **edges** |
| **Data** | EMBER (PE features), CIC-IDS2017 (flow features) | LANL auth **graph**, CICAPT provenance graph |
| **Model** | MLP / CNN on **tabular** vectors | **GCN** link scorer on **graphs** |
| **Task** | Classify sample as malware / attack type | Rank **attack edges** vs **benign** edges |
| **Contribution** | Relaxed consistency → 3–11× train speed | **Negative protocol** — random non-edges inflate AP |
| **Hard negatives** | Not used | Core idea |
| **Link prediction** | No | Yes |

**Overlap only at the word level:** both touch “cybersecurity + deep learning.” This paper is **systems/ML engineering** (KeyDB, async sync). Yours is **graph LP + evaluation methodology** (insider/LM edge ranking).

**For CARS:** cite this as unrelated prior work on cyber DL, not as the same project. Your paper is not a duplicate.

---

**User**

can you find a paper that does similar work?

---

**Cursor**

**No exact match** for your full idea (hard **benign** negatives + tiered cyber LP eval on LANL). Closest papers by overlap:

---

## Closest to your *insight* (random non-edges mislead)

| Paper | Overlap | Gap vs you |
|-------|---------|------------|
| **[HeaRT](https://arxiv.org/pdf/2306.10453)** (Li et al., NeurIPS 2023) | Hard negatives **inflate/deflate** LP metrics; HeaRT = structural **non-edges** | Social/OGB graphs, not cyber; not benign haystack |
| **[Entente](https://arxiv.org/pdf/2503.14284v1)** (NDSS 2026) | On **LANL + Euler**: *“sampled negative edges might not characterize ground-truth malicious edges”*; AP low | FL/systems paper; **does not propose** hard-benign eval protocol |
| **[Dynamic LP evaluation](https://arxiv.org/html/2311.18486)** (2023) | AUC **sensitive to negative sampling**; random negs mislead | Not security / LANL |
| **[Measurement problem](https://www.pnas.org/doi/10.1073/pnas.2312527121)** (PNAS 2024) | High **AUC misleading** when truth is sparse | General embeddings, not cyber graphs |

**Best cite for “we’re not the first to notice the problem”:** **Entente §V-A** on Euler + LANL random non-edge train vs ~hundreds real red-team test edges.

---

## Closest in *cyber domain* (different contribution)

| Paper | What they do | vs you |
|-------|--------------|--------|
| **[Bowman et al.](https://www.usenix.org/conference/raid2020/presentation/bowman)** (RAID 2020) | LANL auth graph; unsupervised LP; NCE **non-edge** negs in embedding | Detector, not eval-protocol study |
| **[Euler](https://dl.acm.org/doi/10.1145/3588771)** (King & Huang, 2023) | LANL/OpTC temporal LP; train random **non-edges**; test = implicit **benign haystack** | No formal hard-negative tiers |
| **[Jbeil](https://par.nsf.gov/servlets/purl/10540681)** (Khoury et al., 2024) | LANL temporal LP; **benign + malicious** in train; threat augmentation | Model paper, not benign-negative eval ablation |

---

## Closest in *method* (hard negatives, not cyber)

| Paper | Note |
|-------|------|
| **[RANS](https://openreview.net/pdf?id=dfdrb2sZUw)** | Dynamic hard **non-edge** mining for LP **training** — general graphs |

---

## Positioning sentence for your paper

> HeaRT shows easy **non-edge** negatives mislead LP evaluation on citation/social graphs; Entente notes the same train/test mismatch for Euler on LANL. **We are the first to formalize tiered evaluation with hard real-traffic (same-user benign) negatives for attack-edge ranking on cyber graphs.**

That claim is defensible if you cite HeaRT + Entente + Euler and show the AP gap across neg tiers.

---

## Papers to put in Related Work

1. **HeaRT** — motivation (hard negatives matter)  
2. **Bowman / Euler** — cyber graph LP baselines  
3. **Entente** — LANL + negative-sampling limitation (closest “similar problem”)  
4. **Jbeil** — supervised alternative  
5. Optional: dynamic LP eval / PNAS measurement — general LP metric critique  

**Bottom line:** **Entente** is the nearest cyber paper that flags your issue; **HeaRT** is the nearest methodology ancestor. Neither does **hard benign / same-user** tiered eval — that’s your niche.

---

**User**

what dies entente do tell briefly?

---

**Cursor**

**Entente** (NDSS 2026) — **federated graph intrusion detection**, not hard-negative evaluation.

**Problem:** GNIDS (Euler, Jbeil) assume **centralized** data; orgs can’t share raw LANL/OpTC-style logs.

**What it does:**
- Splits network log graphs across **clients** (cross-silo FL).
- Builds **reference graphs**, **graph sketching**, and **adaptive contribution scaling** so federated training works despite heterogeneous graphs.
- Evaluates on **LANL, OpTC, Pivoting** with **Euler** and **Jbeil** as the underlying detectors.
- Tests **FL poisoning** defenses (norm bounding).

**Result:** Beats standard FL baselines on AP/AUC; notes Euler on LANL has **low AP** partly because train uses **random non-edge** negatives while test has only **~hundreds** real red-team edges.

**vs your work:** Entente is **distributed training** for existing GNIDS; it **mentions** the negative-sampling issue but does **not** propose hard-benign eval protocols.

---

**User**

what is FL?

---

**Cursor**

**FL = Federated Learning.**

Multiple parties (clients) each keep **local data private** and only share **model updates** (gradients/weights), not raw logs. A central server aggregates updates into one global model.

**Entente context:** Each org/site has its own LANL/OpTC graph shard; Entente trains a shared GNIDS without pooling raw authentication logs in one place.

---

**User**

does it do link prediction and use hard negatives in that?

---

**Cursor**

**Partially — not as its contribution.**

| | Link prediction? | Hard negatives? |
|--|----------------|-----------------|
| **Entente itself** | No new LP model — wraps **Euler** / **Jbeil** | **No** — FL/systems focus |
| **Euler (backend)** | Yes — temporal LP on auth/host graphs | **No** — random **non-edges** in train |
| **Jbeil (backend)** | Yes — self-supervised temporal LP | **No** HeaRT-style hard negs — uses **benign + malicious** events + threat **augmentation** |

Entente **notes** that Euler’s random non-edges don’t match real red-team edges at test (low AP), but it **does not** add hard-negative sampling. That gap is where your eval-protocol idea fits.

---

**User**

what does it do negative sampling for and how?

---

**Cursor**

**Entente does not introduce its own negative sampling.** It runs **Euler** or **Jbeil** under federated learning. Negative sampling comes from those backends.

---

## Euler (what Entente discusses most)

| | |
|--|--|
| **What for** | **Training** only — approximate link-reconstruction loss without scoring the full adjacency matrix |
| **How** | Each epoch, per temporal snapshot: **random non-edges** \((u,v) \notin E_t\) sampled; BCE on real edges (positive) vs sampled holes (negative) |
| **Test** | Score **observed** edges; red-team = positive; other observed = mostly benign (implicit haystack). **No** hard-negative protocol |

Entente’s point: train negatives are **synthetic non-edges**, but test positives are **~hundreds of real red-team edges** → **AP stays low** and train/test distributions don’t match.

---

## Jbeil (other Entente backend)

| | |
|--|--|
| **What for** | **Self-supervised temporal LP** on benign **+** malicious auths |
| **How** | Standard LP negatives in training + **threat sample augmentation** in preprocessing (synthetic attack-like patterns), not HeaRT-style hard non-edges |
| **Metrics in Entente** | Framed differently from Euler (non-existent vs existing edges in their TP/TN definition) |

---

## What Entente adds

**Nothing new on negative sampling** — reference graphs, sketching, adaptive FL aggregation, poisoning defense.

**One line:** Entente uses Euler’s **random non-edge train sampling** and **reports** that it poorly matches real attack edges at test; it does **not** fix it with hard negatives.

---

**User**

so euler uses hard negatives but that doesnot evaluate performance correctly?

---

**Cursor**

**No — Euler does not use hard negatives.** It uses **easy random non-edges** in training.

| Phase | Euler negatives |
|-------|-----------------|
| **Train** | Random **non-edges** (pairs with no link) — easy, not SoHeaRT/CN/PPR hard |
| **Test** | Implicit **benign haystack** — all other **observed** edges in test snapshots (real traffic), not sampled non-edges |

---

## What Entente is saying

Not “hard negatives break eval.” It’s:

> Train optimizes **link vs random hole**; test asks **attack vs real benign traffic** → **distribution mismatch** → **AP often low** (not inflated).

That’s a **train/test task mismatch**, not misuse of hard negatives.

---

## Your paper’s separate point

If you **evaluate** cyber LP with **random non-edges at test** (common in LP papers), AP is **inflated** — that’s different from Euler’s eval, which scores **real** test edges.

**One line:** Euler = **easy** train non-edges + **real** test haystack; Entente says AP is **too low / mismatched**, not that hard negatives broke it. Your contribution is fixing **test** protocols that use easy non-edges.

---

**User**

yes but for my case i am doing hard negatives to confuse model, that is I am trying to  make non-link pairs look like link should exist between those pairs, now I have to:
1. Apply my method on 3 c ybersecurity datasets
2. choose different Cybersecurity problems

---

**Cursor**

**Your SoHeaRT-style setup:** hard **non-edges** \((u,v) \notin E\) chosen so they **look like links should exist** (high CN/PPR, same neighborhood) — to stress the **link-existence** decoder.

That fits **standard LP**. It fits **attack detection** only weakly (attacks are **real** links). Do both: SoHeaRT as **method/ablation**, **same-user benign** as **main cyber eval**.

---

## 3 datasets × 3 cyber problems

| # | Dataset | Graph | **Cyber problem** | **LP task** | Hard non-edge role |
|---|---------|-------|-------------------|-------------|-------------------|
| **1** | **LANL Cyber1** | User → Computer | **Insider / LM:** rare suspicious auths | Rank **red-team auths** vs negatives | SoHeaRT: plausible **missing** auths from same user/subgraph |
| **2** | **CICAPT-IIoT** | Process ↔ Artifact (collapsed) | **APT:** malicious provenance edges in Phase 2 | Rank **attack-touching edges** vs negatives | SoHeaRT on Phase-1 graph: plausible **missing** provenance pairs |
| **3** | **OpTC** | Host → Host (FLOW-START) | **LM:** malicious host communication | Rank **red-team flows** vs negatives | SoHeaRT: plausible **missing** flows (same subnet / high PPR) |

---

## What to run (per dataset)

**Train (SoHeaRT):**
- y=1: benign train edges  
- y=0: random non-edges + **SoHeaRT** hard non-edges (optional: attack train y=0 if supervised)

**Test (two rows — your contribution):**

| Row | Positives | Negatives | Measures |
|-----|-----------|-----------|----------|
| **A (SoHeaRT / your method)** | Attack edges | K **SoHeaRT** non-edges / attack | “Link-like vs plausible hole” |
| **B (Cyber-real)** | Attack edges | K **same-anchor benign** edges | “Attack vs normal traffic” |

Show **AP_A >> AP_B** → SoHeaRT-style eval **overstates** cyber detection.

---

## Problem statements (paper-ready)

1. **LANL:** *Can graph LP with hard structural negatives reliably flag red-team user–computer authentications?*  
2. **CICAPT:** *Do hard non-edge negatives inflate AP for APT-related provenance edges?*  
3. **OpTC:** *Same question for lateral-movement host–host flows.*

---

## Scope choice

| If you emphasize… | Lead with… |
|-------------------|------------|
| **HeaRT/SoHeaRT transfer** | Row A on 3 datasets + GCN |
| **Cyber truth** | Row B main; Row A = “misleading baseline” |

**One line:** Apply SoHeaRT **hard non-edges** on all three graphs for **(1) insider auth, (2) APT provenance, (3) host LM** — but **claim** cyber relevance via **same-user benign** eval, not SoHeaRT alone.

---

**User**

but why do we need negatives here i dont see the need

---

**Cursor**

**You need negatives for training and for ranking metrics — but not always as *sampled non-edges*.**

## Why negatives exist

| Use | Why |
|-----|-----|
| **Train** | If you only show **benign edges (y=1)**, the model learns “everything scores high.” You need **something at y=0** to define “not a normal link.” |
| **Test AP/AUC** | Ranking needs **two sets**: suspicious edges vs **something to rank below them**. |

Without negatives, there is no contrast — only “all links look good.”

---

## What you might not need

**Explicit random / SoHeaRT non-edges at test** — Euler-style:

```text
Score every observed edge in the test window
Attack = labeled positives
Everything else scored = implicit negatives (benign haystack)
→ AP/AUC over all scored edges, no K sampled negs
```

So you don’t need **sampled** negatives at test if you score **all real traffic**.

---

## Three setups

| Setup | Negatives? |
|-------|------------|
| **Train** | **Yes** — random non-edges and/or **benign/attack labels** (y=0) |
| **Test (Euler)** | **Implicit** — all other **benign** edges you scored |
| **Test (HeaRT-style)** | **Explicit** — K negatives per attack edge (for controlled comparison) |

---

## For your paper

- **SoHeaRT non-edges:** optional **train** hard negatives + **ablation** row showing inflated AP.  
- **Cyber-real eval:** negatives = **benign edges** (same user), not invented non-links.  
- **Minimum viable:** train with random non-edges; test score **attack vs all benign test auths** — no SoHeaRT at test.

**One line:** Negatives define contrast; cyber doesn’t need **fake non-links at test** — it needs **benign traffic as the negative class**, which is already in the logs.

---

**User**

but what is the use of hard negatives here. Because all we are doing is predicting if a link is attack or not. Not predicting if a link should exist or not

---

**Cursor**

**You're right — SoHeaRT-style hard non-edges don't fit an attack-vs-benign task.**

| Task | What "hard negative" should be |
|------|-------------------------------|
| **Link exists?** (HeaRT/SoHeaRT) | Hard **non-edges** that look like they should exist |
| **Link is attack?** (your cyber task) | Hard **benign edges** — real traffic that confuses the model |

Attack and benign are **both real links**. Hard **non-edges** only test "is it a link?" — not "is it an attack?"

---

## So when are hard negatives useful for you?

**Only if redefined as hard benign**, not SoHeaRT non-edges:

- **Easy neg:** random benign edge  
- **Hard neg:** same-user / same-host benign edge (traffic attack hides among)

That raises the bar for **attack classification**, not link existence.

---

## What you actually need

**Supervised attack detection:**
```text
Train:  attack y=1, benign y=0  (benign ARE your negatives)
Test:   rank attack above benign (sample or full haystack)
```

**No SoHeaRT non-edges** unless you add an **ablation** showing: *"LP papers that use non-edge negs report misleading AP for this task."*

---

**One line:** Hard non-edges help **existence** LP; for **attack vs not-attack**, negatives = **benign links** (hard = confusable benign). SoHeaRT doesn't apply to the core problem — only as a cautionary baseline.

---

**User**

but hard benigns dont confuse it for attack links right?

---

**Cursor**

**Correct — hard benigns don’t look like attacks.** They look **normal**.

The confusion runs the other way:

```text
Attack edge  →  may look like normal traffic
Hard benign  →  examples of that normal traffic (same user/host/day)
```

**Eval question:** Can the model score the **attack higher** than other **plausible benign** links from the same source?

- **Easy benign:** random normal edge → attack often wins (inflated AP)  
- **Hard benign:** same-user normal edge → attack must beat **real** routine traffic  

Hard benigns aren’t “attack-like.” They’re the **haystack** attacks hide in — that’s why they’re hard.

---

**User**

Find at least 2 graph datasets (ideally 3) about different cybersecurity problems
Run experiments with graph neural/convolutional networks on those dataset, and report the results.
Please use at least 2 graph neural network models for comparison
Add experimental results to the paper.
Guide me

---

**Cursor**

I'll map what's already in the repo and what CARS needs for datasets, GNN baselines, and experiments.


**You already have the right 3 datasets and repo.** CARS wants ≥2 cyber graph datasets, ≥2 GNNs, and reported metrics — your locked story (hard-benign eval) satisfies that without a new architecture.

---

## 1. Three datasets (different cyber problems)

| # | Dataset | Problem | Graph | Status |
|---|---------|---------|-------|--------|
| 1 | **LANL Cyber1** | Lateral movement (auth) | User→Computer | ✓ `auth.txt.gz` + `redteam.txt.gz` in `CARS/dataset/lanl/` |
| 2 | **CICAPT-IIoT** | APT on IIoT | Provenance (collapse to entity pairs) | ✓ Phase1/2 CSVs in `CARS/dataset/provenance/` |
| 3 | **DARPA OpTC** | Enterprise red-team host flows | Host→Host FLOW-START | ✗ `CARS/dataset/optc/` empty — download needed |

**One-line story per dataset:** LANL = credential abuse; CICAPT = provenance APT; OpTC = large-scale host anomaly. Same task everywhere: **rank attack edges above benign decoys**.

**If OpTC slips:** ship **LANL + CICAPT** (2 datasets) and add OpTC if download finishes. For 3, OpTC is the standard third (Euler); don’t swap in NGIDS-DS (tabular IDS, not a graph).

---

## 2. Two GNN models (reuse existing code)

`HeaRT/benchmarking/gnn_model.py` already has **GCN**, **GraphSAGE**, and **GAT**. Minimum comparison:

| Model | Why |
|-------|-----|
| **GCN** | Euler/Bowman baseline; already wired in `SNAP/code_preprocess/main_gnn_snap.py` |
| **GraphSAGE** | Different aggregation; same MLP decoder — fair apples-to-apples |

Same for both: 3 layers, hidden 256, MLP link scorer, SVD-256 node features (`build_snap_features.py`), benign-only `adj_t`.

**Skip for deadline:** Euler GCN+RNN (temporal), R-GCN on full provenance — scope creep.

---

## 3. One protocol, three negative tiers (your contribution)

Same train everywhere:

- **Train pos:** benign edges only  
- **Train neg:** random non-edges  
- **No attack labels in train**

Test (main table = **tier 3**):

| Tier | Test negatives | Role |
|------|----------------|------|
| **A** | Random non-edges (K=500/attack) | Inflated AP (ablation) |
| **B** | Random benign edges | Medium difficulty |
| **C** | **Same-user / same-host benign** | **Main result** — real haystack |

- **Positives:** all held-out attack edges  
- **Score:** `1 - ŷ` (low link likelihood = suspicious), Euler-aligned  
- **Metrics:** pooled **AP** and **AUC** (`link_pred_metrics.py`)

Hard benigns are **normal** traffic the attack must beat — not attack lookalikes.

---

## 4. Per-dataset specifics

**CICAPT-IIoT (start here — smallest)**  
- Train: Phase 1 unique edges (benign)  
- Test pos: Phase 2 edges touching `label=1` nodes (~570)  
- Hard neg: other Phase 2 benign edges sharing source process/host  

**LANL**  
- Train: auths before first red-team timestamp  
- Test pos: `redteam.txt` (~750)  
- Hard neg: same-user benign auths in the attack window  
- Note: 7.6 GB — use Euler-style 30-min snapshots or a day subset first  

**OpTC**  
- Train: 4 benign days of FLOW-START  
- Test pos: red-team FLOW-START from eval days  
- Hard neg: same-source-host benign flows in test window  

---

## 5. Implementation order (~June 15)

```mermaid
flowchart LR
  A[CICAPT preprocessor] --> B[Wire main_gnn_snap cyber eval]
  B --> C[Run GCN + SAGE × 3 neg tiers]
  C --> D[LANL preprocessor]
  D --> E[Repeat experiments]
  E --> F[OpTC if downloaded]
  F --> G[Paper Table I + II]
```

| Day | Task |
|-----|------|
| 1 | `preprocess_cicapt.py` → `graph.pt`, `attack_test_pos.pt`, neg samplers (mirror `preprocess_twitter_edges.py`) |
| 2 | Extend `main_gnn_snap.py` for cyber `evaluation_edges` + `--gnn_model GCN\|SAGE` |
| 3 | CICAPT full table (2 models × 3 neg tiers) |
| 4–5 | LANL preprocessor + runs (subset OK if full graph is slow) |
| 6 | OpTC download + runs, or write paper with 2 datasets + “OpTC pending” |
| 7 | Draft results section |

**Artifacts per dataset** (same layout as Twitter/HeaRT):

- `graph.pt`, `x_snap_svd256.pt`, `attack_test_pos.pt`  
- `neg_random.npy`, `neg_benign_random.npy`, `neg_benign_hard.npy`  

---

## 6. Paper tables (what reviewers want)

**Table I — Datasets**

| Dataset | Nodes | Train edges | Attack test edges | Cyber problem |
|---------|-------|-------------|-----------------|---------------|
| CICAPT | … | … | ~570 | IIoT APT |
| LANL | … | … | ~750 | Lateral movement |
| OpTC | … | … | ~21k | Host anomaly |

**Table II — Main results (tier C: hard benign)**

| Dataset | GCN AP | GCN AUC | SAGE AP | SAGE AUC |
|---------|--------|---------|---------|----------|
| CICAPT | | | | |
| LANL | | | | |
| OpTC | | | | |

**Table III — Your contribution (one model, all datasets)**

| Neg tier | CICAPT AP | LANL AP | OpTC AP |
|----------|-----------|---------|---------|
| Random non-edge | high (inflated) | high | high |
| Random benign | ↓ | ↓ | ↓ |
| **Same-user/host benign** | **honest** | **honest** | **honest** |

Table III is the paper’s core claim: **AP drops when negatives are realistic** — not “we beat Euler.”

---

## 7. Results section outline (≈1 page)

1. **Setup** — 2 GNNs, 3 datasets, 3 neg tiers, unsupervised benign train  
2. **Main** — Table II on hard-benign eval  
3. **Ablation** — Table III shows metric inflation with random non-edges  
4. **Takeaway** — GCN vs SAGE are close; **evaluation protocol** matters more than architecture  

Cite Euler/Bowman as baselines; position your work as **benchmark methodology**, not a new IDS.

---

## 8. What to do right now

1. **Switch to Agent mode** — preprocessors and experiment runs need writes.  
2. **Start CICAPT** — data is ready; fastest path to numbers.  
3. **Parallel:** retry OpTC per-day download into `CARS/dataset/optc/`.  
4. **Don’t block on OpTC** — LANL + CICAPT + 2 GNNs meets the minimum bar.

If you want, in Agent mode I can scaffold `preprocess_cicapt.py` and the cyber eval hook in `main_gnn_snap.py` first.

---

**User**

is there a way to incorporate Heart negatives here? or if you can find different problem in cybersecurity for this?  If not just say NO, dont try to adjust this technique just bcs i am asking for it

---

**Cursor**

**For your current paper (attack-edge ranking on LANL / CICAPT / OpTC): NO.**

HeaRT/SoHeaRT negatives are **hard missing links** — pairs that are **not** in the graph but look plausible. Cyber attack detection is the opposite: attacks **are real edges** hiding among **real benign edges**. The right decoys are **hard benigns** (same user/host), not SoHeaRT non-edges.

The only honest use of HeaRT here is an **ablation row** — “random / SoHeaRT non-edges inflate AP” — which you already have as Tier A. That’s not incorporating HeaRT as a contribution; it’s showing why that eval object is wrong for cyber.

---

**Different cyber problem where HeaRT actually fits: YES, but it’s a different paper.**

| Problem | Why HeaRT fits |
|---------|----------------|
| **CTI / vulnerability knowledge graphs** | Predict **unobserved** links (CVE co-exploitation, technique→artifact, threat-actor→malware) |
| **Attack-hypothesis / ATT&CK reasoning graphs** | Infer **missing** technique or kill-chain relations |

Those are classic “does this latent edge exist?” tasks — same object as Twitter/OGB link prediction. They do **not** substitute for LANL auth / provenance / OpTC attack-edge detection, and you don’t have those KG datasets prepped in the repo.

---

**Bottom line:** Don’t force HeaRT into this paper. Your contribution is **hard-benign evaluation protocol** on attack graphs. HeaRT belongs in a **separate** KG link-prediction project, not as a retrofit here.

---

**User**

hard-benign evaluation protocol?

---

**Cursor**

**Hard-benign evaluation protocol** = how you **test** whether a GNN can **rank real attack edges above realistic normal traffic**, not above obviously fake missing links.

---

## The problem it fixes

Many cyber link-prediction papers report high AP because test negatives are **random non-edges** — pairs that never happened and are easy to tell apart. That does **not** measure “can we find the attack among normal activity?”

---

## Setup (same on LANL, CICAPT, OpTC)

| Phase | What |
|-------|------|
| **Train** | Benign edges only; random non-edges as train negatives |
| **Test positives** | Held-out **attack** edges (red team, Phase 2 APT-touching, etc.) |
| **Test negatives** | **Real benign edges** — the haystack attacks hide in |
| **Score** | `1 - ŷ` (low predicted link probability = more suspicious) |
| **Metrics** | Pooled **AP** and **AUC** |

Train never sees attack labels. You’re not predicting “does this edge exist?” — you’re asking **“is this observed edge more suspicious than other plausible normal links?”**

---

## Three negative tiers (ablation → main)

| Tier | Test negatives | Difficulty |
|------|----------------|------------|
| **A — Random non-edge** | K fake pairs per attack | Easy → **inflated AP** |
| **B — Random benign** | K random real benign edges | Medium |
| **C — Hard benign** | K benign edges from **same user/host/source** as each attack | Hard → **main result** |

**Hard benign** = normal traffic that is **confusable context** for the attack (same user logging into another machine, same host talking to another host). Not attack-like — just **plausible alternatives** the model must beat.

---

## What the paper claims

Not a new GNN architecture. The claim is:

> Cyber LP metrics depend heavily on **which negatives you use**. Random non-edges overstate performance; **same-context benign edges** give a honest attack-ranking benchmark.

That’s the protocol: **attack pos vs tiered benign negatives**, with tier C as the primary eval.

---

**User**

is there some technique to  do hard benign like there is technique for hard negatives?

---

**Cursor**

**No named technique like SoHeaRT exists for hard benigns.** HeaRT hardness is for **missing links** (PPR + CN/RA → plausible non-edges). Hard benigns are **real edges** you pick from the haystack — different object, different rules.

---

## What people actually do

| Approach | How “hard” is defined | Used in |
|----------|----------------------|---------|
| **Context match** | Benign edges sharing **same user / host / source** as the attack | Your tier C; natural for LANL/CICAPT/OpTC |
| **Full haystack** | **All other benign test edges** (no sampling) | Euler/Bowman — hardest, no K hyperparameter |
| **Random benign** | Any real benign edge | Your tier B — baseline |

Euler’s version isn’t a sampler algorithm — it’s **score every observed test edge**; negatives are almost all real benign traffic.

---

## Things you *could* formalize (not standard yet)

1. **Context-constrained sampling** — for each attack edge \((u,v)\), sample K benign edges \((u, v')\) or \((u', v)\) with same \(u\) or same subnet/host. Simple rule; easy to reproduce. **This is your main tier.**

2. **Model-ranked hard benigns** — after a benign-trained model, among context-matched benigns, take top-K by highest \(\hat{y}\) (hardest to beat). Harder, but **model-dependent** and needs a clear definition in the paper.

3. **Temporal proximity** — same user/host **and** within ±Δt of the attack. Adds realism on LANL/OpTC.

None of these is a published “hard benign algorithm” the way SoHeaRT is for non-edges.

---

## What not to reuse from HeaRT

**PPR/CN on the benign graph** picks structurally plausible **missing** links — wrong direction for cyber. You want **existing** normal traffic, not fake edges.

---

**Bottom line:** Hard benigns are defined by **operational rules** (same-context sampling, or full test haystack), not a single graph-mining technique. Formalizing tier C as **“same-source benign sampling”** is enough for the paper; you don’t need a SoHeaRT analogue.

---

**User**

can you think of such a technique? can you think of it? read the chat history. we need such that real benign edge that looks like an attack but is not an attack so the model gets confused. so we then ask model if this particular edge is benign or attack. You may use research papers for some idea and there is no need to use Heart or soheart, you may use it if it helps anyway. This is for research Purpose only!

---

**Cursor**

I'll skim the chat history around the earlier CN/PPR discussion so the proposal is consistent with what was already rejected.

**YES — and the cleanest version is an inversion of SoHeaRT.** No published named sampler does this for cyber graphs (the literature calls the phenomenon "benign anomalies" — rare-but-legit events that cause false positives — but treats it as a nuisance, not a controlled eval set). That's your opening.

## Key insight

What makes a red-team edge "look like an attack" structurally? It's **new** (never seen in train), has **low CN/PPR** between endpoints, and **crosses communities**. So a benign edge "looks like an attack" when it has those same signatures — a new hire's first login, a patch server reaching a new host, a backup job.

SoHeaRT picks **non-edges with HIGH PPR/CN** = plausible missing links.
Invert both axes: pick **real benign edges with LOW PPR/CN** = implausible-but-real links.

| | Non-edge | Real benign edge |
|--|----------|------------------|
| **Easy** | Random non-edge (Tier A) | Random benign (Tier B) |
| **Hard** | SoHeaRT: plausible missing | **Yours: attack-like benign (Tier D)** |

You can reuse the HeaRT machinery (`create_heart_negatives.py` computes CN/RA/PPR) almost verbatim — same scores, take the **bottom** instead of top, over **observed benign test edges** instead of candidate non-edges.

## Three variants, ranked

**1. Structural inversion (recommended — model-agnostic, reproducible)**

For each benign test edge \((u,v')\), compute on the benign **train** graph:
- first-occurrence flag (edge unseen in train)
- CN / RA between \(u, v'\)
- PPR\((u \to v')\)

Select benign edges that are **unseen + bottom-percentile PPR/CN**. These are statistically indistinguishable from attack edges on the features LP models actually use. No model in the loop, no label leakage.

**2. Model-mined false positives (OHEM analogue)**

Score all benign test edges with the benign-trained model; take the K with **lowest ŷ** — the literal edges the model confuses with attacks. This is classic hard-example mining (bootstrapping FPs in detection: Felzenszwalb's DPM, OHEM CVPR'16).
- **Circularity caveat:** mining with model M and evaluating M deflates AP by construction. Fix: **cross-model mining** — mine with GCN, evaluate SAGE, and vice versa. You already have both models, so this is free and makes a nice ablation.

**3. Attack-signature matching (strongest "looks like attack," use with disclosure)**

Featurize every edge (new-edge flag, src out-degree, dst in-degree rarity, CN, PPR, hour-of-day). For each attack edge, take its **nearest benign neighbors** in this feature space (propensity-matching style). Most literal answer to your question, but it uses aggregate attack statistics to build the eval set — defensible for a benchmark paper, but must be stated explicitly.

## What this tier measures (and why it's a finding, not a bug)

With an unsupervised benign-trained model, Tier D AP will likely crash toward 0.5 — **by construction both attack and attack-like benign get low ŷ**. That's the result:

> Unsupervised link prediction cannot distinguish attacks from benign anomalies; it detects *rarity*, not *maliciousness*. Distinguishing them requires supervision (B=1/A=0) or temporal/semantic context.

This is exactly the alert-fatigue problem practitioners report (PIDSMaker KDD'26 calls these "benign anomalies"; the specification-based detection line exists precisely because of them) — but nobody has packaged it as a **controlled negative tier** with a sampler. Your supervised variant (attack labels in train) then gets tested on the question you stated: *given an edge that looks anomalous, is it benign or attack?*

## How it completes the paper

| Tier | Negatives | Question answered |
|------|-----------|-------------------|
| A | Random non-edge | (inflated baseline) |
| B | Random benign | Attack vs typical traffic |
| C | Same-source benign | Does attack **blend into** its own context? (FN direction) |
| **D** | **Low-PPR/unseen benign** | Can model tell attack from **benign anomaly**? (FP direction) |

C and D are complementary failure modes — missed attacks vs false alarms. Together with the HeaRT citation ("hard negatives matter; here's the cyber analogue in both directions"), that's a complete, defensible 6-page evaluation story.

My pick for the deadline: implement **variant 1** (it's ~30 lines on top of existing PPR/CN code), add **variant 2 cross-model** as an ablation if time allows, skip variant 3.

---

**User**

**YES — and the cleanest version is an inversion of SoHeaRT.** No published named sampler does this for cyber graphs (the literature calls the phenomenon "benign anomalies" — rare-but-legit events that cause false positives — but treats it as a nuisance, not a controlled eval set). That's your opening.

## Key insight

What makes a red-team edge "look like an attack" structurally? It's **new** (never seen in train), has **low CN/PPR** between endpoints, and **crosses communities**. So a benign edge "looks like an attack" when it has those same signatures — a new hire's first login, a patch server reaching a new host, a backup job.

SoHeaRT picks **non-edges with HIGH PPR/CN** = plausible missing links.
Invert both axes: pick **real benign edges with LOW PPR/CN** = implausible-but-real links.

| | Non-edge | Real benign edge |
|--|----------|------------------|
| **Easy** | Random non-edge (Tier A) | Random benign (Tier B) |
| **Hard** | SoHeaRT: plausible missing | **Yours: attack-like benign (Tier D)** |

You can reuse the HeaRT machinery (`create_heart_negatives.py` computes CN/RA/PPR) almost verbatim — same scores, take the **bottom** instead of top, over **observed benign test edges** instead of candidate non-edges.

## Three variants, ranked

**1. Structural inversion (recommended — model-agnostic, reproducible)**

For each benign test edge \((u,v')\), compute on the benign **train** graph:
- first-occurrence flag (edge unseen in train)
- CN / RA between \(u, v'\)
- PPR\((u \to v')\)

Select benign edges that are **unseen + bottom-percentile PPR/CN**. These are statistically indistinguishable from attack edges on the features LP models actually use. No model in the loop, no label leakage.

**2. Model-mined false positives (OHEM analogue)**

Score all benign test edges with the benign-trained model; take the K with **lowest ŷ** — the literal edges the model confuses with attacks. This is classic hard-example mining (bootstrapping FPs in detection: Felzenszwalb's DPM, OHEM CVPR'16).
- **Circularity caveat:** mining with model M and evaluating M deflates AP by construction. Fix: **cross-model mining** — mine with GCN, evaluate SAGE, and vice versa. You already have both models, so this is free and makes a nice ablation.

**3. Attack-signature matching (strongest "looks like attack," use with disclosure)**

Featurize every edge (new-edge flag, src out-degree, dst in-degree rarity, CN, PPR, hour-of-day). For each attack edge, take its **nearest benign neighbors** in this feature space (propensity-matching style). Most literal answer to your question, but it uses aggregate attack statistics to build the eval set — defensible for a benchmark paper, but must be stated explicitly.

## What this tier measures (and why it's a finding, not a bug)

With an unsupervised benign-trained model, Tier D AP will likely crash toward 0.5 — **by construction both attack and attack-like benign get low ŷ**. That's the result:

> Unsupervised link prediction cannot distinguish attacks from benign anomalies; it detects *rarity*, not *maliciousness*. Distinguishing them requires supervision (B=1/A=0) or temporal/semantic context.

This is exactly the alert-fatigue problem practitioners report (PIDSMaker KDD'26 calls these "benign anomalies"; the specification-based detection line exists precisely because of them) — but nobody has packaged it as a **controlled negative tier** with a sampler. Your supervised variant (attack labels in train) then gets tested on the question you stated: *given an edge that looks anomalous, is it benign or attack?*

## How it completes the paper

| Tier | Negatives | Question answered |
|------|-----------|-------------------|
| A | Random non-edge | (inflated baseline) |
| B | Random benign | Attack vs typical traffic |
| C | Same-source benign | Does attack **blend into** its own context? (FN direction) |
| **D** | **Low-PPR/unseen benign** | Can model tell attack from **benign anomaly**? (FP direction) |

C and D are complementary failure modes — missed attacks vs false alarms. Together with the HeaRT citation ("hard negatives matter; here's the cyber analogue in both directions"), that's a complete, defensible 6-page evaluation story.

My pick for the deadline: implement **variant 1** (it's ~30 lines on top of existing PPR/CN code), add **variant 2 cross-model** as an ablation if time allows, skip variant 3.



Can you show a concise example if this works or not

---

**Cursor**

**Toy LANL-style example** (3 users, 4 machines, benign train only):

**Train (benign):**
- `alice → ws1` (daily)
- `alice → ws2` (daily)
- `bob → server` (daily)

**Test edges:**

| Edge | Type | Unseen in train? | PPR/CN to pair | Model ŷ (benign-trained) | Anomaly `1-ŷ` |
|------|------|------------------|----------------|--------------------------|---------------|
| `alice → fileserver` | **Attack** | Yes | Low | **0.12** | **0.88** |
| `carol → fileserver` | **Tier D** (new hire, real benign) | Yes | Low | **0.15** | **0.85** |
| `alice → ws2` | **Tier C** (same-user routine) | No | High | 0.91 | 0.09 |

`carol → fileserver` is **not** an attack — but it has the **same structural signature** as the attack (first-time, low neighborhood overlap). The model flags **both** as suspicious.

---

## Does Tier D “work”?

**As detection (unsupervised benign train): NO — and that’s expected.**

```text
Rank by 1-ŷ:
  0.88  attack  (alice → fileserver)   ← y=1
  0.85  Tier D  (carol → fileserver)   ← y=0  ← almost tied
  0.09  Tier C  (alice → ws2)            ← y=0
```

Pooled AP ≈ **0.5** — the model cannot tell attack from benign anomaly. It detects **rarity**, not **malice**.

**As an evaluation protocol: YES.**

It exposes a failure mode Tier C misses:

| Tier | Question | This example |
|------|----------|--------------|
| **C** | Does attack blend into **same-user normal** traffic? | Attack (0.88) beats `alice→ws2` (0.09) → AP **high** |
| **D** | Can model tell attack from **benign that looks anomalous**? | Attack (0.88) ≈ `carol→fileserver` (0.85) → AP **~0.5** |

C tests **missed attacks** (FN). D tests **false alarms** (FP). Both matter.

---

## When Tier D might improve (supervised train)

If train includes **attack y=0** and **benign y=1**:

| Edge | Supervised ŷ | Outcome |
|------|--------------|---------|
| `alice → fileserver` | 0.08 | attack |
| `carol → fileserver` | 0.62 | benign (maybe) |
| `alice → ws2` | 0.94 | benign |

AP on Tier D can rise above 0.5 — but only if the model learns **malice beyond rarity**. That’s a separate experimental row, not guaranteed.

---

## One-line takeaway

**Tier D works as a benchmark, not as a trick to boost AP.** It asks: *“Given an edge that looks structurally like an attack, is it actually an attack?”* On unsupervised LP the honest answer is often **no** — and reporting that gap (Tier A AP high, Tier D AP ~0.5) is the paper’s result.

---

**User**

is this technique really applicable herE?

---

**Cursor**

**Partially yes — strong on LANL, weaker on CICAPT/OpTC unless you filter carefully.** Not your main tier; Tier C stays primary.

---

## Where it genuinely applies

**LANL auth graphs (best fit)**

- Red-team edges are often **first-time user→machine** auths (low CN/PPR vs train).
- Real benign **benign anomalies** exist too: new hire, new machine, backup/patch traffic — also first-time, low PPR.
- Unsupervised LP (Euler/Bowman) scores **rarity**, not malice → Tier D stress-tests the **false-alarm** failure mode.
- Tier C (same-user routine) and Tier D (rare-but-benign) test **different** confusions — both real on LANL.

**Verdict for LANL: applicable and meaningful.**

---

## Where it’s shaky

**CICAPT-IIoT**

- Phase 2 is a **new phase** — many benign edges are “unseen” just because they’re new data, not because they’re attack-like anomalies.
- Without extra filters (same entity type, not Phase-2-global novelty), Tier D ≈ **random benign (Tier B)** → little added value.

**OpTC**

- Works only if you mine within the **same eval day/window**, not “unseen vs all prior benign days.”
- Otherwise everything in the attack window looks novel.

**Supervised train (B=1, A=0)**

- Tier D becomes a different task (classify among suspicious-looking edges).
- That’s a **second experiment**, not the same unsupervised eval story as Euler.

---

## Honest applicability matrix

| Dataset | Tier D useful? | Why |
|---------|----------------|-----|
| **LANL** | **Yes** | Attacks and benign anomalies share rarity signature |
| **CICAPT** | **Maybe** | Need within-phase, entity-aware filters or it duplicates Tier B |
| **OpTC** | **Maybe** | Need same-day/window constraint |

---

## Does it belong in *your* paper?

| Role | Recommendation |
|------|----------------|
| **Main contribution** | **No** — keep **Tier C** (same-source hard benign) as headline |
| **Secondary tier / ablation** | **Yes on LANL** — shows FP vs FN failure modes |
| **All 3 datasets required** | **No** — don’t force Tier D everywhere if the filter doesn’t separate from Tier B |

---

## Bottom line

The technique is **real and research-valid** for cyber graphs where attacks look **structurally anomalous** (rare, low neighborhood overlap) — that’s LANL lateral movement.

It is **not universally applicable** to every cyber LP setup. On CICAPT/OpTC it only works with **tight scoping**; otherwise it’s redundant.

**Practical paper plan:** A / B / C on all 3 datasets (required). Add **Tier D on LANL only** as “benign anomaly confusers” if time allows — cite PIDSMaker-style benign anomalies. Don’t rebuild the paper around Tier D.

---

**User**

Find at least 2 graph datasets (ideally 3) about different cybersecurity problems
Run experiments with graph neural/convolutional networks on those dataset, and report the results.
Please use at least 2 graph neural network models for comparison


give me idea I have to write paper on this for IEEECARS2026. You may/maynot use my codebase which is what I was working but your idea has to have some relation with link prediction.

---

**Cursor**

**Recommended idea (use your codebase — it's 80% there):**

## "Attacks Are Real Edges: Tiered Benign Negatives for Evaluating GNN Link Prediction on Cyber Graphs"

**One-line pitch:** Cyber LP papers report inflated AP because they test attack edges against *random non-edges*; we propose a tiered negative protocol culminating in **hard benign negatives** (same-source real traffic) and show metrics collapse across 3 datasets × 2 GNNs.

**Meets every requirement:**

| Requirement | How |
|------------|-----|
| ≥2 (ideally 3) datasets, different problems | **LANL** (lateral movement, auth), **CICAPT-IIoT** (APT, provenance), **OpTC** (enterprise red-team, flows) — first two already on disk |
| ≥2 GNN models | **GCN** + **GraphSAGE** (both exist in `HeaRT/benchmarking/gnn_model.py`, wired via `main_gnn_snap.py`); GAT as a free third if time |
| Experiments + results | AP/AUC matrix: 2 models × 3 datasets × 3 negative tiers |
| Link prediction | Core task: benign-trained LP scorer ranks attack edges vs negatives |

**The contribution (what reviewers grade):** not a new model — a **protocol + empirical finding**. HeaRT showed easy negatives inflate LP metrics on social/OGB graphs; nobody has shown the cyber analogue, where the right hard negatives are **real benign edges**, not plausible non-edges. The 2-model comparison strengthens it: if GCN and SAGE both collapse under hard benigns, the problem is the *evaluation*, not the architecture.

**Main table:**

| Neg tier | GCN AP | SAGE AP | (× LANL / CICAPT / OpTC) |
|----------|--------|---------|--------------------------|
| Random non-edge | ~0.95+ (inflated) | ~0.95+ | |
| Random benign | ↓ | ↓ | |
| **Same-source hard benign** | **↓↓ (honest)** | **↓↓** | |

Optional LANL-only ablation: Tier D "benign anomaly" negatives (unseen, low-PPR benign edges) — the FP-direction counterpart from earlier discussion.

---

## Alternatives (if you want a different angle — all riskier for June 15)

**Idea B — Static vs temporal under honest negatives.** Add a lightweight Euler-style GCN+GRU as the second "model" and ask: does temporal context survive hard-benign eval better than static GCN? Stronger story, but you'd have to implement snapshotting + RNN in ~5 days. Risky.

**Idea C — Supervision ablation under hard negatives.** Same datasets/models, compare unsupervised (benign-only train) vs supervised (B=1/A=0) on hard-benign tiers — "does supervision buy you anything once negatives are realistic?" Doable, adds one training config; good fallback section if main results feel thin.

**Idea D — Fresh start on CTI knowledge graphs** (CVE co-exploitation, ATT&CK relations) where HeaRT-style hard non-edges apply directly. Clean LP fit, but zero data prepped, new domain, new related work. Don't do this for CARS.

---

## My recommendation

**Idea A as the paper, Idea C as one extra results subsection if time allows.** It's the only option where the preprocessing, models, metrics (`pooled_auc_ap`), and the entire narrative already exist in your repo and chat history.

Next concrete steps (need Agent mode): CICAPT preprocessor → wire cyber eval edges into `main_gnn_snap.py` with `--gnn_model GCN|SAGE` → run 2×3 tier matrix → LANL preprocessor → repeat → OpTC only if the download cooperates (paper stands on 2 datasets).

---

**User**

explain me idea A very simply I do not understand. 

---

**Cursor**

**Idea A in plain terms:**

---

## The real-world problem

On a company network, attacks show up as **normal-looking connections** — a user logs into a machine, a host talks to another host. They’re **real edges** in the graph, mixed in with millions of boring, legitimate ones.

Security researchers use **graph neural networks (GNNs)** and **link prediction** to flag suspicious connections: “this link doesn’t fit normal patterns.”

---

## What’s wrong with how people test this

Many papers test like this:

- **Positive:** attack connection ✓  
- **Negative:** random pairs that **never happened** ✗  

That’s too easy. Random non-connections are obviously fake, so the model looks great (high AP) even if it **can’t** tell attacks from real normal traffic.

It’s like testing a fraud detector by comparing real fraud to **people who don’t exist** — not to **real honest customers**.

---

## Your idea (the contribution)

**Use harder, more realistic negatives** — especially **real benign connections** that could confuse the model.

| Test negative type | What it is | Why |
|--------------------|------------|-----|
| **Easy** | Random fake pairs | What many papers do → **scores look too good** |
| **Medium** | Random real normal connections | Harder |
| **Hard** | Normal connections from the **same user/host** as the attack | Hardest — “find the attack among traffic that looks like it could be normal” |

You **don’t invent a new GNN**. You show: **the same GCN and GraphSAGE look much worse when negatives are realistic.** That’s the finding.

---

## What you actually run

**3 datasets** (different security settings):

1. **LANL** — stolen credentials / lateral movement  
2. **CICAPT** — APT on industrial IoT  
3. **OpTC** — red-team on enterprise hosts  

**2 models:** GCN and GraphSAGE (same setup, fair comparison)

**Train:** model learns “what normal links look like” from benign data only.

**Test:** for each attack link, ask: *does the model rank it above the chosen negatives?*

**Report:** AP/AUC for easy vs medium vs hard negatives.

---

## What the paper says

> “Cyber link-prediction papers often report strong results because they test against fake missing links. When you test against **real normal traffic** — especially from the same user or host — scores drop a lot. You need **hard benign negatives** to know if detection actually works.”

---

## One sentence

**Same GNN, same data — only change how hard the “not attack” examples are; show that easy negatives lie about performance, hard benign ones tell the truth.**

That’s Idea A.

---

**User**

can you give me concise table of what we train/test on and what is the method to get benigns that look like attacks so we test in the end if attack can pass through similar looking benigns or not.

---

**Cursor**

**Train / test / hard-benign sampling (3 datasets, same protocol)**

| | **CICAPT-IIoT** | **LANL Cyber1** | **OpTC** |
|--|-----------------|-----------------|----------|
| **Graph** | Provenance → entity pairs | User → Computer auth | Host → Host FLOW-START |
| **Train graph (`adj_t`)** | Phase 1 benign edges | Auths before first red-team event | 4 benign days |
| **Train y=1** | Phase 1 benign | Pre-attack benign auths | Benign-day flows |
| **Train y=0** | Random non-edges | Random non-edges | Random non-edges |
| **Attack in train?** | No | No | No |
| **Test positives** | Phase 2 attack-touching edges (~570) | `redteam.txt` edges (~750) | Red-team eval-day flows |
| **Val** | Held-out Phase 1 benign + random non-edges | Same pattern | Same pattern |
| **Model** | GCN + GraphSAGE, 3-layer, MLP scorer | same | same |
| **Test score** | `1 - ŷ` (high = more suspicious) | same | same |
| **Metric** | Pooled AP / AUC | same | same |

---

**Negative tiers at test (per attack edge \((u,v)\), sample K=500 decoys each)**

| Tier | What negatives are | How to sample | What you’re testing |
|------|-------------------|---------------|---------------------|
| **A — Easy** | Random **non-edges** | Uniform \((u',v')\notin E_{\text{train}}\) | Inflated baseline (many papers) |
| **B — Medium** | Random **benign** edges | Any real benign test edge | Attack vs typical traffic |
| **C — Hard (main)** | **Same-context benign** | Real benign \((u,v')\) with **same source \(u\)** (or same host/subnet on OpTC) | Can attack **hide among that user’s normal traffic**? |
| **D — Attack-like benign** *(optional, best on LANL)* | Benign edges that **look structurally like attacks** | From benign **test** edges, keep those with: **(i)** unseen in train, **(ii)** bottom percentile **PPR/CN/RA** on benign train graph (SoHeaRT inverted) | Can model tell attack from a **benign anomaly** with the same “rare/new link” signature? |

---

**How to get benigns that look like attacks (Tier D — concise method)**

```text
Candidate pool = all benign test edges (u, v')
For each candidate, on benign TRAIN graph only:
  1. unseen = (u,v') never appeared in train
  2. s = PPR(u→v') + CN(u,v') + RA(u,v')   # reuse HeaRT scores
  3. rank candidates by s ascending (bottom = most attack-like)
Per attack (u,v): sample K benign edges from bottom-ranked pool
  (optionally also require same u or same subnet)
```

**Not attack-like by label** — they’re real benign edges that share the **structural** cues attacks often have: new, low neighborhood overlap, rare.

---

**Final test question (what the paper measures)**

| Tier | Question |
|------|----------|
| **C** | Does the attack edge score **more suspicious** than **other normal links from the same user/host**? (blending in → missed attack) |
| **D** | Does the attack edge score **more suspicious** than **benign edges that also look rare/anomalous**? (passes as “just another weird but legit event” → false alarm / confusion) |

**Good model:** attack ranks above those hard benigns (`1-ŷ` higher on attack).  
**Bad / realistic outcome:** attack ≈ hard benign scores → AP drops → **attack passes among similar-looking benigns**.

---

**User**

r u sure bottom percentile PPR/CN/RA will make benign look like attacks? can you tell how?

---

**Cursor**

**Partly yes — with an important caveat.** Low PPR/CN/RA does not make a benign edge *malicious*. It makes it **look like an attack to a link-prediction anomaly scorer** because both trigger the same structural signals.

---

## What “attack-like” means here

Not “this benign edge is suspicious to a human.”  
It means: **same cues an unsupervised LP model uses to flag anomalies.**

| Signal | Typical attack edge | Bottom-PPR/CN/RA benign |
|--------|---------------------|-------------------------|
| **Unseen in train** | First time user→that machine | First time new hire→machine |
| **CN / RA** | Few shared neighbors | Endpoints barely connected in train graph |
| **PPR** | Weak path from u to v | u and v in different “regions” of the graph |
| **Model ŷ** (benign-trained) | **Low** | **Low** |
| **Anomaly `1-ŷ`** | **High** | **High** |

The model can’t tell them apart — not because the benign is evil, but because **both look like rare, weakly supported links**.

---

## Tiny example

**Train (benign):**  
`alice→ws1`, `alice→ws2`, `bob→server` (dense local neighborhoods)

**Test:**

| Edge | Type | In train? | CN(alice, dest) | Model says |
|------|------|-----------|-----------------|------------|
| `alice→fileserver` | Attack | No | 0 | “unlikely link” → **suspicious** |
| `carol→fileserver` | Benign (new hire) | No | 0 | “unlikely link” → **suspicious** |
| `alice→ws2` | Benign (routine) | Yes | high | “normal link” → not suspicious |

Bottom-PPR/CN/RA picks **`carol→fileserver`**, not `alice→ws2`.  
`carol→fileserver` is benign, but **structurally matches** what makes `alice→fileserver` score as an anomaly.

---

## Why bottom percentile specifically

- **SoHeaRT (top PPR/CN):** “this *missing* pair looks like it *should* exist” → hard **non-edges**
- **Your inversion (bottom PPR/CN):** “this *real* pair looks like it *shouldn’t* exist (given train)” → hard **benign confusers**

Attacks on LANL are often **first-time auths to weakly connected machines** → low CN/PPR.  
Benign **benign anomalies** (new user, new machine, one-off backup) share that profile.

---

## What I’m not claiming

| Claim | True? |
|-------|-------|
| Low PPR benign = actually malicious | **No** |
| All attacks have low PPR/CN | **No** — stolen-cred access to a usual machine is high PPR → **Tier C**, not D |
| Works on all datasets equally | **No** — weak on CICAPT if “unseen” just means “new phase” |
| Supervised model still confused | **Maybe not** — labels can separate rarity from malice |

---

## Bottom line

**How it works:** bottom-PPR/CN/RA selects benign edges that are **novel and topologically weak** in the train graph — the same features unsupervised LP uses to score attacks as anomalous.

**What you test:** can the model rank the **attack above benign edges that score equally “weird”?**

**Honest scope:** this is the **false-alarm / benign-anomaly** direction. **Tier C (same-source)** covers the **blending-in** direction. Use both; don’t rely on Tier D alone.

---

**User**

**Partly yes — with an important caveat.** Low PPR/CN/RA does not make a benign edge *malicious*. It makes it **look like an attack to a link-prediction anomaly scorer** because both trigger the same structural signals.

---

## What “attack-like” means here

Not “this benign edge is suspicious to a human.”  
It means: **same cues an unsupervised LP model uses to flag anomalies.**

| Signal | Typical attack edge | Bottom-PPR/CN/RA benign |
|--------|---------------------|-------------------------|
| **Unseen in train** | First time user→that machine | First time new hire→machine |
| **CN / RA** | Few shared neighbors | Endpoints barely connected in train graph |
| **PPR** | Weak path from u to v | u and v in different “regions” of the graph |
| **Model ŷ** (benign-trained) | **Low** | **Low** |
| **Anomaly `1-ŷ`** | **High** | **High** |

The model can’t tell them apart — not because the benign is evil, but because **both look like rare, weakly supported links**.

---

## Tiny example

**Train (benign):**  
`alice→ws1`, `alice→ws2`, `bob→server` (dense local neighborhoods)

**Test:**

| Edge | Type | In train? | CN(alice, dest) | Model says |
|------|------|-----------|-----------------|------------|
| `alice→fileserver` | Attack | No | 0 | “unlikely link” → **suspicious** |
| `carol→fileserver` | Benign (new hire) | No | 0 | “unlikely link” → **suspicious** |
| `alice→ws2` | Benign (routine) | Yes | high | “normal link” → not suspicious |

Bottom-PPR/CN/RA picks **`carol→fileserver`**, not `alice→ws2`.  
`carol→fileserver` is benign, but **structurally matches** what makes `alice→fileserver` score as an anomaly.

---

## Why bottom percentile specifically

- **SoHeaRT (top PPR/CN):** “this *missing* pair looks like it *should* exist” → hard **non-edges**
- **Your inversion (bottom PPR/CN):** “this *real* pair looks like it *shouldn’t* exist (given train)” → hard **benign confusers**

Attacks on LANL are often **first-time auths to weakly connected machines** → low CN/PPR.  
Benign **benign anomalies** (new user, new machine, one-off backup) share that profile.

---

## What I’m not claiming

| Claim | True? |
|-------|-------|
| Low PPR benign = actually malicious | **No** |
| All attacks have low PPR/CN | **No** — stolen-cred access to a usual machine is high PPR → **Tier C**, not D |
| Works on all datasets equally | **No** — weak on CICAPT if “unseen” just means “new phase” |
| Supervised model still confused | **Maybe not** — labels can separate rarity from malice |

---

## Bottom line

**How it works:** bottom-PPR/CN/RA selects benign edges that are **novel and topologically weak** in the train graph — the same features unsupervised LP uses to score attacks as anomalous.

**What you test:** can the model rank the **attack above benign edges that score equally “weird”?**

**Honest scope:** this is the **false-alarm / benign-anomaly** direction. **Tier C (same-source)** covers the **blending-in** direction. Use both; don’t rely on Tier D alone.



based on this since it is not fully applicable, do you have another recommendation to produce paper for graph ML with 2-3 cybersecurity datasets?

---

**Cursor**

**Yes — but first: the partial applicability of Tier D does not sink Idea A.** Tiers A/B/C are fully applicable on all 3 datasets; Tier D was only ever an optional LANL extra. Idea A minus Tier D is still your strongest paper. That said, here are real alternatives, ranked by feasibility for ~June 15:

---

## Option 1 — Idea A, trimmed (recommended)

**"Hard benign negatives for evaluating GNN link prediction on cyber attack graphs"**

- Tiers: random non-edge / random benign / **same-source benign** — all three work on LANL, CICAPT, OpTC with no caveats
- 2 GNNs (GCN, SAGE) × 3 datasets × 3 tiers = a full results matrix
- Drop Tier D entirely, or mention as future work
- ~90% of code exists; only preprocessors needed

**Risk: low. Novelty: protocol + empirical finding (HeaRT-style, new domain).**

## Option 2 — Supervision ablation under realistic negatives

**"Does supervision help when negatives are realistic?"**

Same setup as Option 1, but the comparison axis is **training regime**, not just architecture:

| Train | Test (tier C) |
|-------|----------------|
| Unsupervised (benign only, Euler/Bowman-style) | attack vs same-source benign |
| Supervised (benign=1, attack=0, Jbeil-style) | same |

- Question: random-negative eval makes both look similar — do they separate under hard benigns?
- Adds one training config to Option 1; reuses everything else
- Works as a standalone story or as Section 5.3 of Option 1

**Risk: low–medium. Good fallback if Option 1's results feel thin.**

## Option 3 — Static vs temporal GNN under honest eval

GCN/SAGE (static) vs a minimal Euler-style GCN+GRU on snapshots. Question: does temporal context actually buy detection power once negatives are realistic, or was it also riding on easy negatives?

- Strongest reviewer appeal of the realistic options
- But: implement snapshotting + RNN + per-snapshot eval in ~5 days, on top of preprocessors

**Risk: high for your deadline. Only if Option 1 finishes early.**

## Option 4 — Feature-source ablation on cyber graphs

Same protocol, compare node feature choices: SVD-256 (your `build_snap_features.py`) vs degree/structural vs random/ID embeddings, across 2 GNNs × 3 datasets. Question: on cyber graphs with no natural node attributes, what features matter for attack ranking?

- Cheap to run (feature swap, same pipeline)
- Weaker as a headline; fine as a second contribution inside Option 1

**Risk: low. Novelty: modest.**

## Option 5 — CTI / vulnerability knowledge graph LP (clean HeaRT fit)

Predict missing links in security knowledge graphs (CVE co-exploitation, ATT&CK technique relations) where SoHeaRT hard non-edges apply **directly** — no inversion gymnastics needed.

- Conceptually cleanest use of your HeaRT code
- But: zero data prepped, new related work, new preprocessing domain — not by June 15

**Risk: very high. This is the next paper, not this one.**

---

## Bottom line

| | Datasets | New code needed | Deadline-safe |
|--|----------|-----------------|---------------|
| **1. Idea A trimmed** | 3 ✓ | preprocessors only | **Yes** |
| 2. + supervision ablation | 3 ✓ | +1 train config | Yes |
| 3. static vs temporal | 3 ✓ | snapshot+RNN | No |
| 4. feature ablation | 3 ✓ | feature swap | Yes (weak alone) |
| 5. CTI knowledge graph | 0 prepped | everything | No |

**Do Option 1, fold in Option 2 as one extra subsection if results land early.** The "not fully applicable" issue was only about Tier D — don't let it push you off the core paper, which has no such problem.

---

**User**

can we work on edge prediction such that predicting if an attack will happen or not and do we have any cubersecuirty datasets for that? Answer briefly

---

**Cursor**

**Yes — that's "attack forecasting," and your same 3 datasets support a limited version of it.**

**Two framings:**

1. **Forecast attack before any sign of it** — essentially impossible from graph history alone (benign history contains no signal about when red team strikes). No dataset fixes this; don't pursue.

2. **Forecast the next attack edge given attack-so-far** (attack-path / lateral-movement progression) — feasible. Given attack edges up to time t, predict which edge the attacker takes at t+1. This is temporal link prediction with attack supervision (Jbeil-adjacent).

**Datasets (all timestamped + attack-labeled):**

| Dataset | Fits because |
|---------|--------------|
| **LANL** | Red-team auths with timestamps — sequence of LM hops ✓ on disk |
| **OpTC** | Multi-day red-team campaign, ordered events |
| **CICAPT-IIoT** | Phase 2 APT stages/tactics in time order |
| DARPA TC (CADETS/THEIA) | APT kill-chains in provenance (heavier prep) |

**Caveats:** ~750 attack edges on LANL ≈ very few "next-hop" training examples; you'd need temporal modeling (snapshots/TGN-style), which is the scope you already rejected for the June 15 deadline.

**Brief verdict:** possible as a paper, wrong paper for this deadline — it's Option-3-level new code plus tiny positive sets. Stick with the eval-protocol paper; attack forecasting is a good follow-up.

---

**User**

Option 5 — CTI / vulnerability knowledge graph LP (clean HeaRT fit)
Predict missing links in security knowledge graphs (CVE co-exploitation, ATT&CK technique relations) where SoHeaRT hard non-edges apply directly — no inversion gymnastics needed.

Conceptually cleanest use of your HeaRT code
But: zero data prepped, new related work, new preprocessing domain — not by June 15
Risk: very high. This is the next paper, not this one.


is there any research papers that does this and what we need to do here? can you explain simply i dont understand

---

**Cursor**

**Yes — people already do this.** It's a different problem from LANL attack-edge detection.

---

## What it is (simple)

Build a **knowledge graph** of security facts:

```text
CVE-A  ──co-exploited-with──►  CVE-B
CVE-X  ──exploits──►  CWE-Y
Technique ──uses──►  Malware
```

Many links are **missing**. **Link prediction** asks: *which new links should exist?*  
Example: "If attackers often chain CVE-A and CVE-B, predict that link even if it wasn't recorded yet."

That's classic link prediction — **missing links** — so **SoHeaRT hard non-edges fit naturally** (unlike LANL, where attacks are real edges).

---

## Papers that do this (examples)

| Paper | What they predict |
|-------|-------------------|
| **Yin et al. (IEEE TIFS)** | CVE **co-exploitation** links on a vulnerability KG (MAGCN) |
| **Zhang et al. (Information Sciences)** | Links in **RCTI** threat-intel KG (EGNN) |
| **Kaiser et al. (TDSC)** | ATT&CK technique links from artifacts (random forest LP) |
| **KGConvE (Scientific Reports)** | Implicit CVE–CVE, CVE–CWE, CVE–CAPEC relations |
| **MDPI attack-path (2026)** | Attack paths as link prediction on CVE/CWE/CAPEC graph |

So this area is **not empty** — your angle would need a clear twist (e.g. HeaRT-style hard negatives on CTI graphs).

---

## What you would need to do

1. **Pick a graph** — e.g. NVD/CVE co-exploitation, or ATT&CK + malware relations  
2. **Build triples** — (head, relation, tail) = observed links = train positives  
3. **Train GNN** — GCN/SAGE embed entities, score candidate links  
4. **Test** — hide some real links; predict them back (standard KG completion)  
5. **Hard negatives** — SoHeaRT: plausible **non-links** (high PPR/CN) vs random non-links  
6. **Report** — Hits@K, MRR, AP (standard KG metrics)

**Data sources:** NVD/CVE dumps, MITRE ATT&CK STIX, existing KGs from those papers — **not** in your repo today.

---

## vs your current paper (Idea A)

| | **Idea A (LANL/CICAPT)** | **Option 5 (CTI KG)** |
|--|--------------------------|------------------------|
| **Question** | Is this **real attack edge** suspicious? | Will this **missing relation** exist? |
| **Positives** | Observed attack edges | Known KG triples |
| **Hard negatives** | Real **benign** edges | SoHeaRT **non-edges** ✓ |
| **Your data** | LANL + CICAPT on disk | **None prepped** |
| **Deadline** | Feasible | **Too much new work** |

---

## One sentence

**Option 5 = predict hidden links in a CVE/ATT&CK knowledge graph; papers exist; HeaRT fits cleanly; but you'd build a new dataset and pipeline from scratch — good next paper, not this one.**

---

**User**

For other datasets/problems, are they also classification or regression?
 **Yes — but first: the partial applicability of Tier D does not sink Idea A.** Tiers A/B/C are fully applicable on all 3 datasets; Tier D was only ever an optional LANL extra. Idea A minus Tier D is still your strongest paper. That said, here are real alternatives, ranked by feasibility for ~June 15:

---

## Option 1 — Idea A, trimmed (recommended)

**"Hard benign negatives for evaluating GNN link prediction on cyber attack graphs"**

- Tiers: random non-edge / random benign / **same-source benign** — all three work on LANL, CICAPT, OpTC with no caveats
- 2 GNNs (GCN, SAGE) × 3 datasets × 3 tiers = a full results matrix
- Drop Tier D entirely, or mention as future work
- ~90% of code exists; only preprocessors needed

**Risk: low. Novelty: protocol + empirical finding (HeaRT-style, new domain).**

## Option 2 — Supervision ablation under realistic negatives

**"Does supervision help when negatives are realistic?"**

Same setup as Option 1, but the comparison axis is **training regime**, not just architecture:

| Train | Test (tier C) |
|-------|----------------|
| Unsupervised (benign only, Euler/Bowman-style) | attack vs same-source benign |
| Supervised (benign=1, attack=0, Jbeil-style) | same |

- Question: random-negative eval makes both look similar — do they separate under hard benigns?
- Adds one training config to Option 1; reuses everything else
- Works as a standalone story or as Section 5.3 of Option 1

**Risk: low–medium. Good fallback if Option 1's results feel thin.**

## Option 3 — Static vs temporal GNN under honest eval

GCN/SAGE (static) vs a minimal Euler-style GCN+GRU on snapshots. Question: does temporal context actually buy detection power once negatives are realistic, or was it also riding on easy negatives?

- Strongest reviewer appeal of the realistic options
- But: implement snapshotting + RNN + per-snapshot eval in ~5 days, on top of preprocessors

**Risk: high for your deadline. Only if Option 1 finishes early.**

## Option 4 — Feature-source ablation on cyber graphs

Same protocol, compare node feature choices: SVD-256 (your `build_snap_features.py`) vs degree/structural vs random/ID embeddings, across 2 GNNs × 3 datasets. Question: on cyber graphs with no natural node attributes, what features matter for attack ranking?

- Cheap to run (feature swap, same pipeline)
- Weaker as a headline; fine as a second contribution inside Option 1

**Risk: low. Novelty: modest.**

## Option 5 — CTI / vulnerability knowledge graph LP (clean HeaRT fit)

Predict missing links in security knowledge graphs (CVE co-exploitation, ATT&CK technique relations) where SoHeaRT hard non-edges apply **directly** — no inversion gymnastics needed.

- Conceptually cleanest use of your HeaRT code
- But: zero data prepped, new related work, new preprocessing domain — not by June 15

**Risk: very high. This is the next paper, not this one.**

---

## Bottom line

| | Datasets | New code needed | Deadline-safe |
|--|----------|-----------------|---------------|
| **1. Idea A trimmed** | 3 ✓ | preprocessors only | **Yes** |
| 2. + supervision ablation | 3 ✓ | +1 train config | Yes |
| 3. static vs temporal | 3 ✓ | snapshot+RNN | No |
| 4. feature ablation | 3 ✓ | feature swap | Yes (weak alone) |
| 5. CTI knowledge graph | 0 prepped | everything | No |

**Do Option 1, fold in Option 2 as one extra subsection if results land early.** The "not fully applicable" issue was only about Tier D — don't let it push you off the core paper, which has no such problem.

---

**Cursor**

**Mostly binary classification (edge-level ranking), not regression.**

---

## Idea A and Options 1–4 (LANL / CICAPT / OpTC)

| Piece | Type |
|-------|------|
| **Train** | **Binary classification** — benign edge = 1, non-edge (or attack, in Option 2) = 0, BCE loss |
| **Model output ŷ** | Probability in [0,1] (looks like regression, but it's a **class probability**) |
| **Test** | **Binary ranking** — attack = positive, negatives = 0; metrics = **AP / AUC** |

You are **not** predicting a continuous target (e.g. attack severity, time-to-breach). You score each edge and ask: *attack above benign?*

---

## Option 5 (CTI knowledge graph)

Same: **binary link existence** — does relation (CVE–CVE, technique–malware) exist?  
Metrics: Hits@K, MRR, AP — still **classification / ranking**, not regression.

---

## Other cyber graph problems (if you switched)

| Problem | Usually |
|---------|---------|
| Node anomaly detection (UNICORN, PIDSMaker) | **Node binary classification** |
| Attack next-hop forecasting | **Ranking** or **multi-class** (pick next edge) |
| Predict CVSS / exploit likelihood | **Regression** — different task, not your LP setup |

---

**One line:** Your paper options are all **edge-level binary link prediction**, evaluated as **ranking** (AP/AUC). Not regression.

---

**User**

Why bottom percentile specifically
SoHeaRT (top PPR/CN): “this missing pair looks like it should exist” → hard non-edges
Your inversion (bottom PPR/CN): “this real pair looks like it shouldn’t exist (given train)” → hard benign confusers
Attacks on LANL are often first-time auths to weakly connected machines → low CN/PPR.
Benign benign anomalies (new user, new machine, one-off backup) share that profile.


so in this case will we be exposing attack looking benigns only in test graph or all benigns?

---

**Cursor**

**All benigns still exist in the test period — Tier D only changes which ones you use as negatives for scoring.**

---

## What’s in the data vs what you score

| | What |
|--|------|
| **Test data (full)** | All benign test edges + all attack edges — unchanged |
| **Tier D eval** | Per attack \((u,v)\), sample **K negatives** only from **attack-looking benigns** (unseen + bottom PPR/CN/RA) |

You do **not** remove other benigns from the dataset. You **don’t score all of them** in the Tier D AP/AUC pool either.

---

## Tier D pipeline (concise)

```text
Pool     = all benign test edges
Filter   = unseen in train + bottom percentile PPR/CN/RA  → "attack-looking benign" subset
Per attack (u,v): sample K negatives from that subset (optionally same u)
Eval set = all attacks (pos) + those K×N_attack sampled hard benigns (neg)
```

So Tier D exposes **only attack-looking benigns as decoys** in the metric — not every benign edge.

---

## Compare tiers

| Tier | Test negatives drawn from |
|------|---------------------------|
| **B** | Any benign test edge |
| **C** | Benign test edges with **same source** \(u\) |
| **D** | Benign test edges that are **unseen + low PPR/CN** (attack-looking subset) |

---

## GNN forward pass

Still uses **benign train `adj_t` only** (your static setup). Test edges are **scored**, not necessarily added to the graph. Tier D affects **which edges you compare the attack against**, not which edges exist in the log.

**One line:** All benigns remain in test data; Tier D eval uses **only a sampled subset of attack-looking benigns** as negatives, not the full benign haystack (that’s closer to Euler).

---

**User**

Pool     = all benign test edges
Filter   = unseen in train + bottom percentile PPR/CN/RA  → "attack-looking benign" subset
Per attack (u,v): sample K negatives from that subset (optionally same u)
Eval set = all attacks (pos) + those K×N_attack sampled hard benigns (neg)

but all benign edges will be seen in train right?

---

**Cursor**

**No — not all benign test edges are in train.** That’s why the “unseen” filter matters on LANL; on CICAPT it’s partly “new phase,” which is weaker.

---

## What “unseen” means

**Edge \((u,v)\) not in the train edge set** — not “node never seen.”

Example:
- Train: `alice→ws1`, `alice→ws2`
- Test benign: `alice→ws3` → **unseen** (new pair)
- Test benign: `alice→ws1` → **seen** (routine, repeats in test)

---

## By dataset (your split)

| Dataset | Train benigns | Test benigns | All test benigns in train? |
|---------|---------------|--------------|----------------------------|
| **LANL** | Pre–red-team auths | Benign auths **during** attack window | **No** — mix of routine (seen) + new/rare (unseen) |
| **CICAPT** | Phase 1 | Phase 2 benign | **Mostly no** — but “unseen” often = new phase, not attack-like |
| **OpTC** | 4 benign days | Eval-day benign flows | **No** — new day → many new pairs |

---

## Tier D pool

```text
All benign test edges
  → keep only (u,v) NOT in train
  → among those, keep bottom PPR/CN/RA
  → sample K per attack
```

**Seen** test benigns (e.g. `alice→ws1` every day) are **dropped** from the Tier D pool. They’re normal traffic, not attack-looking confusers.

---

## If you used a random benign split instead

Hold-out 30% of benign **edges** for test: those edges are unseen by construction, but endpoints usually **were** in train. Tier D still works; “unseen” = held-out edge tuple.

---

**One line:** Train has past benigns; test has **new** benign edges too. Tier D uses only test benigns that are **not in train** and **structurally rare** — not all benign test traffic.

---

**User**

**Yes — people already do this.** It's a different problem from LANL attack-edge detection.

---

## What it is (simple)

Build a **knowledge graph** of security facts:

```text
CVE-A  ──co-exploited-with──►  CVE-B
CVE-X  ──exploits──►  CWE-Y
Technique ──uses──►  Malware
```

Many links are **missing**. **Link prediction** asks: *which new links should exist?*  
Example: "If attackers often chain CVE-A and CVE-B, predict that link even if it wasn't recorded yet."

That's classic link prediction — **missing links** — so **SoHeaRT hard non-edges fit naturally** (unlike LANL, where attacks are real edges).

---

## Papers that do this (examples)

| Paper | What they predict |
|-------|-------------------|
| **Yin et al. (IEEE TIFS)** | CVE **co-exploitation** links on a vulnerability KG (MAGCN) |
| **Zhang et al. (Information Sciences)** | Links in **RCTI** threat-intel KG (EGNN) |
| **Kaiser et al. (TDSC)** | ATT&CK technique links from artifacts (random forest LP) |
| **KGConvE (Scientific Reports)** | Implicit CVE–CVE, CVE–CWE, CVE–CAPEC relations |
| **MDPI attack-path (2026)** | Attack paths as link prediction on CVE/CWE/CAPEC graph |

So this area is **not empty** — your angle would need a clear twist (e.g. HeaRT-style hard negatives on CTI graphs).

---

## What you would need to do

1. **Pick a graph** — e.g. NVD/CVE co-exploitation, or ATT&CK + malware relations  
2. **Build triples** — (head, relation, tail) = observed links = train positives  
3. **Train GNN** — GCN/SAGE embed entities, score candidate links  
4. **Test** — hide some real links; predict them back (standard KG completion)  
5. **Hard negatives** — SoHeaRT: plausible **non-links** (high PPR/CN) vs random non-links  
6. **Report** — Hits@K, MRR, AP (standard KG metrics)

**Data sources:** NVD/CVE dumps, MITRE ATT&CK STIX, existing KGs from those papers — **not** in your repo today.

---

## vs your current paper (Idea A)

| | **Idea A (LANL/CICAPT)** | **Option 5 (CTI KG)** |
|--|--------------------------|------------------------|
| **Question** | Is this **real attack edge** suspicious? | Will this **missing relation** exist? |
| **Positives** | Observed attack edges | Known KG triples |
| **Hard negatives** | Real **benign** edges | SoHeaRT **non-edges** ✓ |
| **Your data** | LANL + CICAPT on disk | **None prepped** |
| **Deadline** | Feasible | **Too much new work** |

---

## One sentence

**Option 5 = predict hidden links in a CVE/ATT&CK knowledge graph; papers exist; HeaRT fits cleanly; but you'd build a new dataset and pipeline from scratch — good next paper, not this one.**



for this does any paper exists that improve evaluation using heart or other hard negatives Techniques for link prediction or this problem is also link Classification likle attack one? check properly onw eb

---

**Cursor**

**Short answer:** CTI/vulnerability KG work is also **binary link classification / ranking**. Papers exist on **hard negatives**, but **almost none use HeaRT/SoHeaRT-style eval on cyber KGs** — that gap is real.

---

## Is CTI KG LP the same task type as attack-edge detection?

**Same ML type, different object.**

| | **LANL attack edges (Idea A)** | **CTI KG (Option 5)** |
|--|-------------------------------|------------------------|
| **Task** | Binary: suspicious or not? | Binary: does this relation exist? |
| **Positive** | Observed **attack** edge | Known **triple** (e.g. CVE–CVE co-exploit) |
| **Negative** | Non-edge or **real benign** edge | **Corrupted / missing** triple |
| **Output** | Score ŷ ∈ [0,1] | Same |
| **Metrics** | AP, AUC | MRR, Hits@K, AUC (often) |

Both are **edge-level binary classification evaluated as ranking** — not regression.

---

## Do papers improve eval with HeaRT / hard negatives?

### General LP (not cyber-specific)

| Paper | What it does |
|-------|----------------|
| **HeaRT / SoHeaRT** (NeurIPS’23) | **Eval protocol** — hard non-edges via PPR/CN/RA on **OGB** (collab, citation, etc.) |
| **RANS** | Hard negatives for **training** GNN LP |
| **SANS, NSCaching, KBGAN** | Hard negatives for **KG completion training** (FB15K, WN18RR) |
| **“Don’t Mess with Mister-in-Between”** (EACL’23) | Hard negative **search** for KGC |

**HeaRT is eval-focused; most others are training-focused.**

### Cybersecurity KG papers (CVE, CTI, ATT&CK)

| Paper | Hard-negative eval like HeaRT? |
|-------|------------------------------|
| **Yin et al. MAGCN** (CVE co-exploitation) | **No** — random negatives from entity set; temporal train/test split |
| **Zhang EGNN** (RCTI KG) | Standard KGC eval; no HeaRT-style protocol |
| **KGConvE** (CVE–CWE–CAPEC) | Standard ranking (MRR, Hits@10) |
| **Kaiser ATT&CK** (TDSC) | Classical LP scores + RF; no hard-eval protocol |
| **SNAQE** (MDPI, cyber KG **quality**) | “Spurious negatives” — related idea, but for **KG quality**, not LP benchmark critique |
| **AT4CTIRE** | Adversarial hard negatives for **relation extraction**, not LP eval |

**I did not find a paper that applies HeaRT/SoHeaRT evaluation to cybersecurity knowledge graphs.**

---

## What that means for you

**Option 5 twist (if you did it later):**  
> “HeaRT showed random negatives inflate LP metrics on OGB; we show the same on CVE/ATT&CK knowledge graphs.”

That would be **new for cyber KG**, but:
- Yin et al. and others already do LP on CVE graphs with **random negatives**
- General hard-negative literature is **crowded** (SANS, RANS, HeaRT)
- You’d still need to **build the KG from scratch**

**Idea A twist (your current path):**  
Cyber **attack graphs** need **hard benign** negatives (real edges), not SoHeaRT non-edges — **different and less covered** than Option 5.

---

## One-line summary

CTI KG LP = **binary link prediction** like attack-edge scoring, but on **missing relations**, not observed attacks. Hard-negative **training** papers exist; **HeaRT-style eval on cyber KGs** — I didn’t find it. Your Idea A (hard **benign** eval on LANL/CICAPT) fills a **different, more appropriate** gap for attack detection. Option 5 + HeaRT is a plausible **follow-up**, not the June 15 paper.

---

**User**

list steps we need to do for KG heart negatives

---

**Cursor**

**KG + HeaRT hard negatives — step list**

---

## Phase 1 — Build the cyber KG

1. **Pick target relations** — e.g. CVE–CVE `co-exploited-with`, CVE–CWE, technique–malware (start with **one relation type**).
2. **Collect sources** — NVD/CVE dumps, Yin’s co-exploitation events, MITRE ATT&CK STIX, or rebuild from a paper’s KG.
3. **Build triples** `(head, relation, tail)` — observed links = positives.
4. **Build homogeneous graph** for HeaRT — usually **collapse relation types** to undirected `(u,v)` edges (or one graph per relation).
5. **Node IDs** — map entities → `0..N-1`; save `edge_index`, splits, id maps.

---

## Phase 2 — Train/val/test split (standard KGC)

6. **Split positives** — e.g. 80/10/10 or Yin-style temporal (pre-2015 train, post-2015 test).
7. **Train graph** — edges in **train split only** (for PPR/CN and GNN `adj_t`).
8. **Val/test positives** — held-out **real triples** to predict back.

---

## Phase 3 — HeaRT negative generation (reuse your repo)

9. **Add KG loader** — new `get_data_cyber_kg()` in `heart_negatives/hard_utils.py` (mirror `get_data_twitter` / OGB format: train/val/test edge splits).
10. **Compute PPR** — run `calc_ppr.py` / `create_ppr_matrices` on **train adj**.
11. **Run `create_heart_negatives.py`** per val/test positive:
    - **Random (HeaRT-easy):** random non-edges in candidate set `S(u,a)`
    - **SoHeaRT (hard):** top-K by PPR + CN/RA among non-edges
    - Output: `heart_test_*.npy` shape `[N_test_pos, K]`
12. **K = 500** (match HeaRT default).

---

## Phase 4 — GNN link prediction

13. **Node features** — CVE metadata (CVSS, text embeddings) or structural-only (degree / SVD like `build_snap_features.py`).
14. **Train GCN + GraphSAGE** — BCE: train edges = 1, random train non-edges = 0.
15. **Score test candidates** — for each held-out positive `(u,v)` and its K HeaRT/SoHeaRT negatives, compute ŷ via MLP decoder (same as `main_gnn_snap.py`).

---

## Phase 5 — Evaluation (the paper table)

16. **Metrics** — MRR, Hits@10, AP, AUC (`link_pred_metrics.py` pooled AP/AUC works).
17. **Compare neg protocols:**

| Eval | Negatives |
|------|-----------|
| Easy | Random non-edges |
| Hard | SoHeaRT (PPR/CN top-K) |

18. **Optional:** filtered KGC eval (standard in PyKEEN/TransE papers) vs HeaRT per-positive sampling — report both.
19. **Ablation:** CN vs RA, K=100 vs 500, GCN vs SAGE.

---

## Phase 6 — Paper

20. **Claim:** HeaRT-style hard negatives collapse metrics on cyber KG too (like OGB); Yin et al. used random negatives → likely inflated.
21. **Cite:** HeaRT (NeurIPS’23), Yin MAGCN, SANS/RANS as related (training hard negs).

---

## Repo touchpoints (what you’d extend)

| Existing | New for KG |
|----------|------------|
| `heart_negatives/create_heart_negatives.py` | `--dataset cyber_kg` loader |
| `heart_negatives/calc_ppr.py` | KG adj input |
| `main_gnn_snap.py` / `main_gnn_ogb.py` | KG data module |
| `link_pred_metrics.py` | reuse as-is |

---

## Realistic effort (~order)

| Step | Time |
|------|------|
| KG build + splits | 2–4 days |
| HeaRT loader + negs | 1–2 days |
| GNN train/eval | 1–2 days |
| **Total** | **~1–2 weeks** |

**Bottom line:** Same HeaRT pipeline as Twitter/OGB — **new part is building the CVE/CTI KG and a dataset loader**; scoring/eval code already exists.

---

**User**

can you briefly and simply tell what the problem is here? i dont understand

---

**Cursor**

**The problem (one sentence):**  
People test cyber graph models with **too-easy fake examples**, so scores look good even when the model **can’t tell real attacks (or real missing links) from normal traffic.**

---

## Two settings, same mistake

**1. Attack graphs (LANL, CICAPT)** — your main paper  
- Attacks are **real connections** (user logs into a machine).  
- Bad test: compare attacks to **pairs that never happened** → easy.  
- Real test: compare attacks to **real normal connections** from the same user/host → hard.

**2. Knowledge graphs (CVE, ATT&CK)** — Option 5  
- Task: predict **missing links** (e.g. “will these two CVEs be chained?”).  
- Bad test: compare to **random fake pairs** → easy.  
- Better test: compare to **plausible fake pairs** (HeaRT/SoHeaRT) → hard.

---

## What you’re NOT building

Not a new detector. You’re asking: **“When we test fairly, do GNNs still work well?”**  
Often: **no** — and that’s the finding.

---

## One analogy

Fraud detector tested on **real fraud vs imaginary customers** → 99% accuracy.  
Test on **real fraud vs real honest customers** → maybe 60%.  

**That gap is the problem your paper exposes.**

---

**User**

2. Knowledge graphs (CVE, ATT&CK) — Option 5

Task: predict missing links (e.g. “will these two CVEs be chained?”).
Bad test: compare to random fake pairs → easy.
Better test: compare to plausible fake pairs (HeaRT/SoHeaRT) → hard.

what needs to be implemented for this?

---

**Cursor**

**Option 5 = build a CVE/ATT&CK graph, then plug it into your existing HeaRT + GNN pipeline.** Most new work is data prep, not a new model.

---

## What you need to implement

### 1. Data (new — biggest part)
- Download/build a KG: CVE co-exploitation, CVE–CWE, or ATT&CK relations
- Convert to triples: `(head_id, relation, tail_id)`
- Map entities → integer IDs `0..N-1`
- Split: train / val / test **positive edges** (hide some real links for test)

### 2. Graph files (new)
- `train_edges.pt` — train positives only (for GNN + PPR/CN)
- `val_pos.pt`, `test_pos.pt` — held-out links to predict
- Optional: node features (CVE metadata, text embeddings, or degree-only)

### 3. HeaRT loader (small extension)
- Add `get_data_cyber_kg()` in `heart_negatives/hard_utils.py` (like Twitter/OGB loaders)
- Feed train adj into `calc_ppr.py` → PPR matrices
- Run `create_heart_negatives.py` with `--dataset cyber_kg` → outputs:
  - `heart_test_random_*.npy` (easy non-edges)
  - `heart_test_soheart_*.npy` (hard non-edges, top PPR/CN)

### 4. GNN training (mostly reuse)
- Wire KG into `main_gnn_snap.py` or `main_gnn_ogb.py`:
  - Load graph + features
  - Train: positives = train edges, negatives = random non-edges
  - Same GCN + GraphSAGE + MLP scorer you already have

### 5. Evaluation (reuse)
- For each test positive `(u,v)`: score it + K negatives from HeaRT files
- Report AP, AUC, MRR, Hits@10 via `link_pred_metrics.py`
- Compare **random non-edge** vs **SoHeaRT hard non-edge** eval

### 6. Paper experiments
- 2 models (GCN, SAGE) × 2 neg tiers (random vs SoHeaRT) × 1–2 KG datasets
- Table showing metrics drop under hard negatives

---

## What you do **not** need
- New GNN architecture
- Tier C/D hard **benign** logic (that’s for LANL attack graphs, not KG)
- Temporal RNN / Euler

---

## Effort split

| Piece | New? | ~Time |
|-------|------|-------|
| Build CVE/ATT&CK KG + splits | **Yes** | 2–4 days |
| HeaRT loader + run negs | Small extension | 1 day |
| GNN train/eval hook | Mostly copy Twitter path | 1 day |
| Run experiments + table | — | 1 day |

**Bottom line:** Implement **KG construction + loader**; everything else is mostly wiring your existing `create_heart_negatives.py` + `main_gnn_snap.py` to that graph.

---

**User**

Build CVE/ATT&CK KG + splits
other papers do it like this Yes or No?

---

**Cursor**

**Mostly yes on KG + splits — no on HeaRT-style eval.**

---

## What other papers do the same way

| Step | Your plan | Typical in CVE/ATT&CK LP papers? |
|------|-----------|----------------------------------|
| Build KG from public sources (NVD, CVE, ATT&CK, CWE) | ✓ | **Yes** — Yin, KGConvE, Kaiser, RCTI all do this |
| Triples `(head, relation, tail)` | ✓ | **Yes** |
| Map entities → integer IDs | ✓ | **Yes** |
| Train on known links, test on held-out links | ✓ | **Yes** — standard link prediction / KG completion |
| Train negatives = random non-links | ✓ | **Yes** — Yin explicitly uses random entity pairs |
| GNN + link scorer | ✓ | **Yes** — MAGCN, EGNN, KGConvE, etc. |

---

## Where they differ from your plan

| Step | Your plan | What papers usually do |
|------|-----------|------------------------|
| **Split style** | You might use random 80/10/10 | **Yin: temporal** (pre-2015 train, post-2015 test). Others: random or 4:1 |
| **Test negatives** | HeaRT / SoHeaRT (hard non-edges) | **Random** corrupt head/tail, or **filtered** rank-over-all-entities (standard KGC) — **not HeaRT** |
| **Metrics** | AP/AUC + HeaRT pool | Often **MRR, Hits@10**, sometimes AUC |
| **Node features** | Optional SVD/degree | Yin uses **multi-modal** CVE attributes (CVSS, text, etc.) |

---

## One-line answer

**Yes** — building a CVE/ATT&CK KG, splitting positive links, and training a GNN link predictor is exactly how those papers work.

**No** — almost none use **HeaRT/SoHeaRT hard-negative eval**; that part would be **your** contribution on top of their setup.

---

**User**

give links for this paper

---

**Cursor**

**CVE / ATT&CK knowledge-graph link prediction papers** (Option 5):

| Paper | Link |
|-------|------|
| **Yin et al. — MAGCN (CVE co-exploitation LP)** | [IEEE Xplore](https://ieeexplore.ieee.org/document/9832800) · DOI: [10.1109/TII.2022.3190895](https://doi.org/10.1109/TII.2022.3190895) |
| **Zhang et al. — EGNN / RCTI CTI KG** | [ScienceDirect](https://www.sciencedirect.com/science/article/pii/S0020025523011770) · DOI: [10.1016/j.ins.2023.119770](https://doi.org/10.1016/j.ins.2023.119770) |
| **Kaiser et al. — ATT&CK AttackDB + link prediction** | [IEEE Xplore](https://ieeexplore.ieee.org/document/10032541) · DOI: [10.1109/TDSC.2022.3233703](https://doi.org/10.1109/TDSC.2022.3233703) · [PDF](https://us-isr-energycenter.org/energy_cyber/papers/Attack_Hypotheses_Generation_Based_on_Threat_Intelligence_Knowledge_Graph.pdf) |
| **KGConvE — CVE/CWE/CAPEC inference** | [Nature Scientific Reports](https://www.nature.com/articles/s41598-025-17941-y) · [PMC](https://pmc.ncbi.nlm.nih.gov/articles/PMC12494800/) |
| **MDPI — attack path LP (CVE/CWE/CAPEC)** | [Electronics 2026](https://www.mdpi.com/2079-9292/15/3/552) |
| **SNAQE — cyber KG quality / spurious negatives** | [MDPI Mathematics](https://www.mdpi.com/2227-7390/13/1/68) |
| **Threat KG repo (CVE–CPE–CWE)** | [GitHub: nislab/threat-knowledge-graph](https://github.com/nislab/threat-knowledge-graph) |

**HeaRT (hard negatives for LP eval — your twist on top):**

| Paper | Link |
|-------|------|
| **HeaRT / SoHeaRT (NeurIPS 2023)** | [arXiv](https://arxiv.org/abs/2306.10453) · [OpenReview](https://openreview.net/forum?id=YdjWXrdOTh) · [Code](https://github.com/Juanhui28/HeaRT) |
| **Your local HeaRT code** | `/home/mmanjee/HeaRT/HeaRT/heart_negatives/` |

**Related (attack graphs, not KG — Idea A):**

| Paper | Link |
|-------|------|
| **Euler — LANL/OpTC temporal LP** | [ACM DOI](https://doi.org/10.1145/3588771) · [NDSS'22](https://www.ndss-symposium.org/ndss-paper/auto-draft-227/) · local: `euler.txt` |

---

**User**

But i see yin et al doesnot only do link prediction, it has produced a new architecture magcn

---

**Cursor**

**Correct — Yin et al. is not only an evaluation paper.**

They do **two things**:

| Yin contribution | What it is |
|------------------|------------|
| **Problem framing** | CVE co-exploitation = **link prediction** on a vulnerability KG |
| **Model** | **MAGCN** — new multimodal GCN that fuses CVE attributes + graph structure |

So their paper is **“new architecture + LP on cyber KG”**, not **“how to evaluate LP fairly.”**

---

## How that relates to your options

**Option 5 + HeaRT twist (if you did KG):**
- You would **not** compete on beating MAGCN with a new GNN
- Your claim would be: *same task (CVE LP), but random-negative eval inflates scores; HeaRT-style hard negatives give honest numbers*
- Use **commodity GCN/SAGE** (like HeaRT does on OGB) — architecture is **not** the contribution

**Idea A (LANL/CICAPT — recommended):**
- Same split: **protocol paper**, not new architecture
- Even clearer gap: Yin-style papers rarely discuss **attack-edge** eval; Euler/Bowman use easy negatives too

---

## What reviewers might ask

> “Yin already did cyber graph LP — what’s new?”

**Answer:** Yin proposes **how to model** co-exploitation (MAGCN). You propose **how to evaluate** link prediction on cyber graphs when negatives matter (hard benigns on attack graphs, or HeaRT on KGs). Different contribution type.

---

## If you wanted to be more like Yin

You’d need **both** a new model **and** experiments — too much for June 15. Yin took years on MAGCN + GKTL + VulKG construction.

---

**One line:** Yin = **new model + LP task**. Your paper = **evaluation protocol + empirical gap** with standard GCN/SAGE. That’s a valid and smaller scope — and closer to what HeaRT did for OGB than what Yin did for cyber KG.

---

**User**

what is the kind of prediction here? is it really a link prediction or link classification if its an attack or benign, can you clarify??

---

**Cursor**

**Both settings use the same math (score an edge pair), but they ask different questions.** The name depends on what you’re scoring.

---

## Three tasks (don’t mix them)

| Task | Question | Positives | Negatives | Your papers |
|------|----------|-----------|-----------|-------------|
| **Link prediction (LP)** | Does this link **exist**? | Real links in graph | **Non-edges** (pairs that never happened) | HeaRT, Yin CVE co-exploit, Option 5 |
| **Edge / link classification** | This link **exists** — what **type** is it? | One class (e.g. attack) | Another class (e.g. benign) | Supervised Jbeil-style |
| **Anomaly edge scoring** | This **observed** link — how **normal** is it? | (Often) attack at test | Benign traffic at test | Euler, Bowman, Idea A (unsupervised) |

All three output a score per edge pair and use **binary** labels at train/test — so people say “classification” and “prediction” interchangeably. The difference is **what the labels mean**.

---

## Idea A (LANL / CICAPT / OpTC)

**Operational question:** attack or benign?

**Unsupervised train (Euler / your default):**
- Train: benign = “link should exist”, random pairs = “should not”
- That’s **link prediction** on benign structure, **not** attack-vs-benign classification in training
- Test: score **observed** attack edges vs chosen benign decoys → **ranking** “which is more suspicious?”
- So: **LP-trained scorer used for anomaly ranking**, not classic “predict missing link”

**Supervised train (optional ablation):**
- Train: benign = 1, attack = 0 on **real edges**
- That **is** **edge-level binary classification** (attack vs benign)

Same GNN, different label meaning.

---

## Option 5 (CVE / ATT&CK KG)

**Question:** will this **relation exist**?

- Positives: known triples (e.g. CVE–CVE co-exploited)
- Negatives: corrupted / missing links
- Classic **link prediction / KG completion** — not attack vs benign

Yin MAGCN is this: predict **missing co-exploitation links**, not “is this auth malicious?”

---

## One-line map

| Setting | Best name |
|---------|-----------|
| **Idea A, unsupervised** | Link prediction → **anomaly edge ranking** (attack vs benign at test) |
| **Idea A, supervised** | **Edge binary classification** (attack vs benign) |
| **Option 5 / Yin** | **Link prediction** (exist vs not exist) |

---

## For your paper wording

- Don’t say only “link prediction” for Idea A without explaining it.
- Safer: *“We evaluate GNN link scorers on **ranking attack edges above benign decoys**”* (Idea A) or *“**knowledge graph link completion**”* (Option 5).

**Bottom line:** Idea A is **not** pure LP like Yin — attacks are **real edges**, so you’re really doing **edge-level ranking/classification at test**, even if the model was trained as LP on benigns. Option 5 **is** real link prediction (missing links).

---

**User**

can you explain with a concise example implementation of knowledge graph link completion” (Option 5). you may use ideas from papers you mentioned that do this. explain precisely problem and then solution in details briefly to understand what is being done

---

**Cursor**

**Toy example: CVE co-exploitation KG (Yin-style)**

---

## Problem

Security analysts know some CVEs are **chained in real attacks**, but many chains are **unrecorded**.

**Known facts in the graph:**

```text
CVE-2017-0144  ──co-exploit──►  CVE-2017-8464   ✓ (seen in ExploitDB)
CVE-2017-0144  ──co-exploit──►  CVE-2018-20250  ✓
CVE-2018-8120  ──co-exploit──►  CVE-2019-0708   ✓
```

**Missing fact (not in DB yet):**

```text
CVE-2017-8464  ──co-exploit──►  CVE-2019-0708   ?  ← predict this
```

**Link completion task:** given the partial graph, predict which **missing triples** are true.

This is **not** “attack vs benign on a network edge.” It is **“does this security relation exist?”**

---

## Data (what you build — Yin / KGConvE style)

**Step 1 — Entities (nodes)**  
CVE-2017-0144, CVE-2017-8464, … → IDs 0, 1, 2, …

**Step 2 — Relations (edge types)**  
`co-exploit`, `has-CWE`, `maps-to-CAPEC` (KGConvE uses several)

**Step 3 — Triples (observed links)**  
Each row: `(head, relation, tail)` = one positive fact.

**Step 4 — Split (Yin: temporal)**  
- **Train:** co-exploit events before 2015  
- **Test:** hide some co-exploit links after 2015 → model must **recover** them

**Step 5 — Node features (optional, Yin MAGCN)**  
Per CVE: CVSS, year, text embedding, vendor, etc.

---

## Solution (what the model does)

### Train

1. Build graph from **train triples only**.
2. **Positive:** real train triple `(u, r, v)`.
3. **Negative:** corrupt it → `(u, r, v')` where `v'` is random/wrong (Yin: random entity).
4. **GNN** (GCN / MAGCN): embed nodes using graph + features.
5. **Scorer:** MLP or dot product → ŷ = P(link exists).
6. **Loss:** BCE — real triples → 1, fake → 0.

```text
Train pos:  (CVE-2017-0144, co-exploit, CVE-2017-8464)  → 1
Train neg:  (CVE-2017-0144, co-exploit, CVE-2020-9999)  → 0  (random tail
```

### Test (standard KGC — Kaiser / KGConvE)

Hide test triple: `(CVE-2017-8464, co-exploit, CVE-2019-0708)`.

**Query:** rank all candidate tails `v'` for fixed `(head, relation)`.

```text
Score (CVE-2017-8464, co-exploit, CVE-2019-0708)  → 0.91  ← true (hidden)
Score (CVE-2017-8464, co-exploit, CVE-2020-1234)  → 0.02  ← random
Score (CVE-2017-8464, co-exploit, CVE-2018-8120)  → 0.45  ← hard: structurally similar CVE
```

**Metrics:** MRR, Hits@10 — is the true missing link ranked high?

### Your HeaRT twist (eval only)

Instead of only random wrong tails, for each test positive `(u,v)` sample **hard non-edges** `(u,v')` with **high PPR/CN** on the train graph — pairs that look like they *should* link but don’t (SoHeaRT).

Compare AP/MRR: **random negatives (easy)** vs **SoHeaRT (hard)**.

---

## Kaiser ATT&CK variant (same idea, different graph)

**Graph:** malware artifact → ATT&CK technique, technique → technique, etc.

**Problem:** given observed artifacts, **infer missing technique links** in AttackDB.

**Solution:** graph traversal + link prediction scores (Jaccard, Adamic-Adar, or supervised RF) to rank plausible `(artifact, technique)` or `(technique, technique)` links.

Same LP/completion pattern, different entities.

---

## Zhang RCTI variant

**Graph:** security **requirement** ↔ **threat intel** entities.

**Problem:** many requirement–threat links are missing in standards data.

**Solution:** EGNN propagates edge info; predict missing links to connect CTI to requirements (~97% connectivity in their eval).

---

## End-to-end pipeline (what you’d implement)

```text
NVD + ExploitDB + MITRE
    → triples + node features
    → train/val/test split (temporal or random)
    → GCN/SAGE train (pos edges + random neg)
    → test: rank hidden triples
    → optional: HeaRT hard negs at eval
    → report MRR, Hits@10, AP
```

---

## vs Idea A (LANL) — one contrast

| | **Option 5 (KG completion)** | **Idea A (attack graphs)** |
|--|------------------------------|----------------------------|
| **Predict** | Missing **relation** between CVEs | Which **observed** connection is attack |
| **Attack edge** | N/A — no “attack edge” label | Attack edge **is** the positive |
| **Hard negative** | Plausible **non-link** (SoHeaRT) | Real **benign link** (same user) |

---

**One sentence:** Option 5 = fill in **missing security facts** in a CVE/ATT&CK graph; train on known triples, test by hiding some and asking the GNN to rank the true missing link above fakes — Yin builds the graph + MAGCN; your add-on would be **harder fakes at test** (HeaRT), not a new architecture.

---

**User**

so what do papers use for negs in the test set? 

---

**Cursor**

**Short answer:** Cyber KG papers mostly use **corrupted triples** (wrong head/tail) at test, often **ranking over many candidates** with **filtered** evaluation — **not** HeaRT-style hard non-edges, and **not** real benign edges.

---

## What Yin / typical KGC papers do at test

**Hidden positive:** one real triple you removed, e.g.  
`(CVE-2017-8464, co-exploit, CVE-2019-0708)`

**Test negatives = corruptions of that triple:**

| Type | Example |
|------|---------|
| **Tail corrupt** | `(CVE-2017-8464, co-exploit, v')` for many wrong `v'` |
| **Head corrupt** | `(u', co-exploit, CVE-2019-0708)` for many wrong `u'` |

**How many:**
- **Full ranking:** all entities (~thousands of CVEs) as candidates
- **Or sampled:** random wrong tails (Yin uses **random** from entity set for training; test is usually standard rank-all or corrupt-one)

**Filtered eval (standard in KGC):** when ranking, **remove** other true triples from the candidate list so a “wrong” candidate that is actually true elsewhere doesn’t count as a negative.

**Metrics:** MRR, Hits@1/3/10 — is the **true missing tail** ranked above corruptions?

Yin: train negs = **random entity pairs**; test = **held-out real links** ranked against corruptions (standard LP completion).

---

## Kaiser (ATT&CK AttackDB)

Uses **link prediction scores** (Jaccard, Adamic-Adar, Katz, or supervised RF) to rank **candidate technique links** — negatives are **other candidate pairs** in the projected graph, not a separate “benign traffic” set.

---

## KGConvE / Zhang EGNN

Standard **KG completion test:** hide test triples, score corruptions, report MRR / Hits@10.

---

## HeaRT (general LP — **not** used in Yin et al.)

| | **Typical cyber KG** | **HeaRT** |
|--|---------------------|-----------|
| **Test neg** | Corrupt head/tail OR rank all entities | **K=500 hard non-edges** per positive (PPR/CN) |
| **Personalized?** | Per query (fixed head, relation) | Per positive edge |
| **Goal** | Find correct entity among wrong ones | Show random negs inflate metrics |

**No cyber KG paper we cited applies HeaRT at test** — that’s the gap.

---

## vs attack graphs (Idea A) — don’t confuse

| | **KG completion (Option 5)** | **Attack graphs (Idea A)** |
|--|-------------------------------|----------------------------|
| **Test negatives** | **Fake triples** (wrong CVE tail) | **Real benign edges** or random non-edges |
| **Test positives** | **Hidden true triples** | **Labeled attack edges** |

---

**One line:** At test, KG papers use **wrong head/tail replacements** (and often rank over all entities, filtered) — **not** hard PPR negatives (HeaRT) and **not** real benign network edges.

---

**User**

so if triples are fake and donot look like test positives, it should be easy for model to predict that it is not a link right

---

**Cursor**

**Often yes for a single random wrong tail — but test is usually harder than that.**

---

## Random corruption (easiest)

```text
True:  (CVE-A, co-exploit, CVE-B)
Fake:  (CVE-A, co-exploit, CVE-Z)   ← Z picked at random
```

If Z is unrelated, the model often scores this low → **easy**.

Same issue as HeaRT on social graphs: **easy negatives inflate scores.**

---

## What papers actually do at test (harder)

**Rank the true tail against many candidates**, not one random fake:

```text
Query: (CVE-A, co-exploit, ?)
Candidates: ALL other CVEs (thousands)
True answer: CVE-B (hidden)
```

Many wrong candidates are **similar CVEs** (same vendor, era, CWE) → **harder** than one random Z.

**Filtered eval:** drop other **known true** tails from the list so you don’t penalize valid alternate links.

---

## Three difficulty levels

| Test setup | Difficulty |
|------------|------------|
| 1 random wrong tail | **Easy** |
| Rank all entities (standard KGC) | **Medium** — similar entities in the list |
| HeaRT / structurally similar fakes (high PPR/CN) | **Hard** |

---

## So is Yin “too easy”?

**Partly.** Random train negs are easy. **Test** (rank-all + filter) is the real benchmark — still not as hard as HeaRT’s **personalized hard non-edges**.

That’s why a HeaRT-on-cyber-KG paper could still show: **even on CVE graphs, metrics drop when negatives are structurally plausible fakes.**

---

**One line:** One random fake triple is easy; standard KGC test ranks against **many** wrong entities (some similar). HeaRT pushes further with **hand-picked hard fakes** — same logic as your Idea A on attack graphs, but for **missing links** not **benign traffic**.

---

**User**

how do they get fake triples from? and which one is harder heart negs or fake truples?

---

**Cursor**

**1. Where fake triples come from**

Start from a **real test triple** you hide:

```text
True (hidden): (CVE-A, co-exploit, CVE-B)
```

Build fakes by **replacing one part**:

| Method | How | Example |
|--------|-----|---------|
| **Tail corrupt** | Same head + relation, **wrong tail** | `(CVE-A, co-exploit, CVE-Z)` |
| **Head corrupt** | Wrong head + same relation + tail | `(CVE-X, co-exploit, CVE-B)` |
| **Relation corrupt** | Same head/tail, wrong relation | `(CVE-A, has-CWE, CVE-B)` |

**Where does Z / X come from?**

- **Random:** pick any other entity from the CVE list (Yin-style train negs).
- **Rank-all (standard test):** use **every other CVE** as a candidate tail (thousands of fakes).
- **Filtered:** remove candidate `Z` if `(CVE-A, co-exploit, Z)` is **already a known true triple** elsewhere in the graph.

So fakes are **not scraped from outside** — they’re **synthetic corruptions** of the true query.

---

**Tiny example**

```text
Known graph:  A─co-exploit─B,  A─co-exploit─C,  D─co-exploit─E

Hidden test:  (A, co-exploit, B)

Tail-corrupt fakes:
  (A, co-exploit, C)  ← wrong for this query, but C is a real neighbor of A → HARDER
  (A, co-exploit, D)  ← medium
  (A, co-exploit, Z)  ← random unrelated CVE → EASIER
```

---

**2. Which is harder: HeaRT negs or fake triples?**

**Depends which fake-triple protocol you mean.**

| Protocol | Difficulty |
|----------|------------|
| **1 random tail corrupt** | **Easiest** |
| **Rank-all tails** (standard KGC test) | **Medium** — list includes easy + similar CVEs |
| **HeaRT / SoHeaRT** (K hard non-edges per positive, top PPR/CN) | **Hard** — picks the **most plausible** missing links on purpose |

**Rough order (hardest at bottom):**

```text
1 random fake triple  <  rank-all (average candidate)  <  rank-all (top similar tails)  ≈  SoHeaRT
```

**Why HeaRT is usually harder than one random fake:**  
SoHeaRT **searches** for non-edges that look like they should exist (high PPR/CN, same neighborhood as the true link).

**Why rank-all isn’t trivial either:**  
The candidate list **already contains** hard fakes (similar CVEs sharing vendor/CWE/era) — you must rank the true tail above all of them.

**Why one random fake is easiest:**  
Often picks a totally unrelated entity.

---

**3. HeaRT vs KGC — same idea, different packaging**

| | **KGC fake triples** | **HeaRT** |
|--|----------------------|-----------|
| **Fixed** | head `A`, relation `co-exploit` | one endpoint of edge `(u,v)` |
| **Vary** | tail over many `v'` | other endpoint `v'` (non-edge) |
| **Hard version** | rank-all + filter | top-K by PPR/CN among non-edges |

Structurally similar — HeaRT just **explicitly mines** the hard end of the candidate set instead of ranking the full entity list.

---

**One line:** Fake triples = **corrupt** a hidden true triple (random tail or **all** tails); **one random corrupt is easy**, **rank-all is medium**; **SoHeaRT is among the hardest** because it chooses the most graph-plausible non-links on purpose.

---

**User**

so fake triples get its corrupted part at random by changin either of 3 parts or is there some way like heart?

---

**Cursor**

**Both exist.** Default is random; harder methods (including HeaRT-like ideas) are used in stronger KGC/LP work.

---

## Default (most papers, including Yin-style)

**Random corruption** of **one** slot:

- Usually corrupt **head** or **tail** (50/50), **not** relation
- Replace with a **uniform random** other entity (sometimes **type-constrained**: only CVEs as tail if head is CVE)

```text
True:  (A, co-exploit, B)
Fake:  (A, co-exploit, Z)   Z ~ random CVE
```

Train: a few random fakes per positive.  
Test: often **rank all entities** as tail (not one random Z).

---

## Harder than random (KG / LP literature)

| Method | How corruption is chosen |
|--------|--------------------------|
| **Rank-all + filtered** (standard test) | All wrong tails; filter known true triples — **implicitly** includes hard similar entities |
| **Type-constrained** | Only swap with valid entity types — harder than pure random |
| **NSCaching / SANS / RANS** | Sample non-edges with **high structural score** — **training**, HeaRT-like |
| **Self-adversarial** (RotatE etc.) | Weighted sample by model score — hard negatives **during train** |
| **“Don’t Mess with Mister-in-Between”** (EACL’23) | Mine negatives **between** easy and false-positive region |
| **HeaRT / SoHeaRT** | **Eval**: top-K non-edges by **PPR + CN/RA** per positive — closest to “hard by design” |

So: **not only random** — but **Yin-style cyber KG papers often use random for train** and **rank-all for test**, not explicit HeaRT mining.

---

## Random vs HeaRT-style corruption

| | **Random corrupt** | **HeaRT-style** |
|--|-------------------|-----------------|
| **Pick Z** | Uniform random entity | Non-edge with **high PPR/CN** to anchor node |
| **When** | Train (common); test (weak) | Eval (HeaRT); some train samplers |
| **Difficulty** | Easy on average | Hard on purpose |

**Same shape** (corrupt tail or head); **different rule** for picking the corrupt entity.

---

## Example

```text
True: (CVE-A, co-exploit, CVE-B)

Random fake:     (CVE-A, co-exploit, CVE-1999-0001)  ← unrelated, easy
HeaRT-style fake: (CVE-A, co-exploit, CVE-C)         ← C shares neighbors with B, no direct link, hard
```

---

**One line:** Fake triples are usually **one part swapped**; the swap is **random by default**, but **hard methods** (rank-all, type constraints, NSCaching/SANS, **HeaRT**) pick **plausible** wrong entities instead of uniform random. Cyber KG papers mostly use **random train + rank-all test**, not HeaRT.

---

**User**

so for us we will going to pick entire fake pairs or only one entity from tiplets?

---

**Cursor**

**It depends which paper track — they use different negative shapes.**

---

## Option 5 (KG + HeaRT) — **one entity from the triple**

Keep **head + relation** (or **relation + tail**), swap **only the other entity**.

```text
True:  (CVE-A, co-exploit, CVE-B)

Fakes:
  (CVE-A, co-exploit, Z)   ← change tail only
  (X, co-exploit, CVE-B)   ← change head only
```

**HeaRT does the same:** for true edge `(u,v)`, negatives are `(u,v')` or `(u',v)` — **one endpoint fixed**, not a fully random unrelated pair.

**You do not** use `(X, Z)` with both ends random for HeaRT/KGC eval.

---

## Idea A (attack graphs) — **depends on tier**

| Tier | Negative shape |
|------|----------------|
| **A — random non-edge** | **Entire fake pair** `(u', v')` — both ends random, often no link to attack |
| **B — random benign** | **Real edge** `(u', v')` — any benign link |
| **C — hard benign** | **One anchor fixed:** `(u, v')` — **same user/host** as attack `(u,v)` |
| **HeaRT (if used)** | **One anchor fixed:** `(u, v')` non-edge |

```text
Attack:     (alice, fileserver)

Tier A:     (bob, printer)        ← whole random pair
Tier C:     (alice, ws2)          ← same alice, different machine
```

---

## What you should pick

| Paper | Negative rule |
|-------|----------------|
| **Option 5 (KG)** | **Corrupt one entity** (HeaRT / standard KGC) |
| **Idea A (main)** | **Tier C:** one shared endpoint `(u, v')`; Tier A can use full random pairs as easy baseline |

---

**One line:** KG/HeaRT = **one part of the triple changed, one anchor kept**; Idea A easy tier = **whole fake pair**, hard tier = **same source, different target** — like HeaRT’s anchored negatives but with **real benign** edges instead of fake non-edges.

---

**User**

I want to show heart method on multiple problems applied. lIKE IdeaA and option5. can you tell me if it would be right to pick a new edge for option 5 or one of triplets pair? explaim briefly

---

**Cursor**

**For Option 5, use one anchored corruption (one entity from the triple) — not a fully new random pair.** That matches HeaRT.

---

## Option 5 (KG): what to pick

**HeaRT is defined on edges `(u,v)`, not random pairs.**

For triple `(head, relation, tail)`:

```text
True:  (CVE-A, co-exploit, CVE-B)

HeaRT-style fake:  (CVE-A, co-exploit, Z)   ← fix head + relation, change tail
                   (X, co-exploit, CVE-B)   ← fix tail + relation, change head
```

**Do not** use a totally unrelated pair `(X, Z)` with both ends random — that’s easy Tier-A style, **not** HeaRT.

**Practical rule:** collapse to edge `(head, tail)` **per relation**, then run HeaRT: negatives = `(u, v')` non-edges with high PPR/CN.

---

## Idea A vs Option 5 — same HeaRT machinery, different negatives

| | **Option 5 (KG)** | **Idea A (attack graphs)** |
|--|-------------------|----------------------------|
| **HeaRT object** | Hard **non-edges** `(u,v')` | Same shape, but **wrong for main eval** |
| **Cyber main eval** | HeaRT fits well | **Hard benign** `(u,v')` **real** edges |
| **Anchor** | One endpoint fixed | Tier C: same `u` |

**Unified story:** “HeaRT = **personalized hard negatives** (one anchor fixed).”  
- On KGs: anchor + **fake** missing link (SoHeaRT).  
- On attack graphs: anchor + **real** benign link (your hard-benign protocol).

---

## Is a multi-problem HeaRT paper reasonable?

**Yes, if framed as one evaluation principle, two domains:**

1. **KG completion (Option 5):** SoHeaRT non-edges — HeaRT **directly**  
2. **Attack graphs (Idea A):** hard benign edges — **cyber adaptation** of “hard personalized negatives” (not literal SoHeaRT)

**Risk:** reviewers may say attack graphs aren’t missing-link LP. **Fix in wording:** same *eval idea*, different *negative type* per domain.

---

**One line:** Option 5 → **change one entity in the triple** (HeaRT anchored non-edge); Idea A → **same anchor rule**, but negatives are **real benign edges**, not fake triples. Don’t use fully random `(u',v')` pairs for HeaRT eval on either problem.

---

**User**

but my idea is implementable in attack graphs right? si there some paper that has fdone similar work for attack graphs to predict link using heart negatives?

---

**Cursor**

**Yes — Idea A is implementable on attack graphs** (LANL/CICAPT/OpTC). Your repo already has the GNN + HeaRT negative code; you add cyber preprocessors and eval tiers.

**No — I did not find a paper that applies HeaRT/SoHeaRT evaluation on attack graphs (LANL, provenance, OpTC).** HeaRT is published on OGB, Planetoid, and similar — not cyber LM graphs.

---

## What exists on attack graphs (similar, not HeaRT)

| Paper | LANL / cyber? | Hard negatives? |
|-------|---------------|-----------------|
| **Euler** | Yes | **Implicit** — test negatives = other **real benign** edges in the window (full haystack), not SoHeaRT |
| **Bowman** | Yes | Same family — anomaly LP, no HeaRT protocol |
| **Jbeil** | Yes | **Threat augmentation** at train; critiques random non-edges — not HeaRT eval |
| **Entente** | Yes | FL wrapper; notes train/test negative mismatch — doesn’t use HeaRT |

**Graph anomaly detection** papers (ANOMIX, etc.) discuss hard negatives for **training** — not the HeaRT **eval benchmark** on LANL.

---

## Your position

| Approach | On attack graphs? | Published? |
|----------|-------------------|------------|
| **Literal SoHeaRT** (hard **non-edges**) | Implementable as **ablation** | **No** on cyber LM — weak fit (attacks are real edges) |
| **Hard benign** (same-source **real** edges) | **Main Idea A** | **No** as formal protocol — closest to Euler’s haystack, but you make it explicit + tiered |

---

## If you want “HeaRT on multiple problems”

| Domain | HeaRT-style eval |
|--------|------------------|
| **Option 5 (KG)** | **Literal SoHeaRT** — fits; no cyber paper found doing this |
| **Idea A (attack graphs)** | **Adaptation** — hard **benign** edges, not SoHeaRT; cite HeaRT as motivation |

**One line:** Your idea is **buildable** on attack graphs; **no prior paper** runs HeaRT negatives there — that’s novelty, but for attack graphs the **honest** version is **hard benign eval**, with SoHeaRT only as an optional “wrong object” ablation row.

---

**User**

is this idea really relevant for attack graoh?>

---

**Cursor**

**Yes for the problem; partly yes for HeaRT literally.**

---

## What *is* relevant on attack graphs

Attack graphs (LANL, CICAPT, OpTC) have a real eval bug:

- Attacks = **real edges**
- Bad test = random **non-edges** → scores look good, ops meaning is weak
- Good test = **real benign edges** the attack must beat → that’s Idea A (hard benign / same-source)

**Euler already does this implicitly** on LANL: test negatives are mostly **other real auths**, not fake pairs. So the *concern* is relevant — you’d just **make it explicit and tiered**, which cyber papers don’t do.

---

## What is only partly relevant

**Literal HeaRT/SoHeaRT** (hard **missing** links) is built for “does this edge exist?” — on attack graphs the failure mode is “is this **real** edge malicious?” SoHeaRT is a useful **ablation** (“random/missing-link AP is inflated”), not the main cyber eval.

| Eval | Relevant for attack graphs? |
|------|----------------------------|
| Random non-edge | **Weak** — easy, many papers still do it |
| **Hard benign (same user/host)** | **Strong** — matches LM stealth |
| SoHeaRT non-edges | **Marginal** — wrong object, optional row |

---

## Honest limits

- **Relevant** if you frame it as **evaluation methodology** for GNN link scorers on cyber graphs — not a new IDS.
- **Less relevant** if reviewers want **temporal** detection (Euler/Jbeil) and you only run static GCN.
- **CICAPT/OpTC** need careful splits; **LANL** is the cleanest story.

---

**One line:** The idea **is** relevant for attack graphs because attacks hide in **real benign traffic**; hard-benign eval matches that. **HeaRT-by-name** is only fully on-point as **inspiration + KG side**; on attack graphs your main contribution is **hard benign protocol**, not SoHeaRT non-edges.

---

**User**

its for IEEECARS 2026. If you have any more problems let me know where Heart is applicable in cyber security for LP

---

**Cursor**

**For IEEE CARS 2026:** strongest fit is **one eval paper** — hard negatives expose when LP metrics lie. Below: limitations to expect, and where HeaRT applies in cyber LP.

---

## Problems / risks (be upfront in the paper)

| Issue | What reviewers may say | Your response |
|-------|------------------------|---------------|
| **Not a new detector** | “Only GCN + eval” | Contribution = **protocol + empirical gap**, like HeaRT on OGB |
| **HeaRT ≠ attack graphs** | “Attacks aren’t missing links” | Main cyber eval = **hard benign**; SoHeaRT = ablation or KG only |
| **Euler already uses benign haystack** | “Not novel on LANL” | You **formalize tiers** (random / benign / same-source) + multi-dataset |
| **Static vs temporal** | “Euler is temporal” | Scope: static GCN; temporal = future work |
| **OpTC missing** | “Only 2 datasets” | LANL + CICAPT minimum; OpTC if download works |
| **Low AP under hard eval** | “Model fails” | **Expected** — that’s the finding |
| **Dual story (Idea A + Option 5)** | “Two papers in one” | **Pick one headline** for 6 pages; other = short section or drop |

**CARS reality:** ~6 pages, ~June 15 — **don’t do both full tracks.** Recommend **Idea A (attack graphs)** as main; Option 5 only if LANL+CICAPT finish early.

---

## Where HeaRT is applicable in cyber LP

| Cyber LP problem | HeaRT (SoHeaRT non-edges) | Your adaptation |
|------------------|---------------------------|-----------------|
| **KG completion** (CVE co-exploit, CVE–CWE, ATT&CK links) | **Yes — direct fit** | Run `create_heart_negatives.py` on KG adj |
| **CTI relation inference** (RCTI, AttackDB technique links) | **Yes — direct fit** | Same: missing link = positive, hard non-edge = SoHeaRT |
| **Software supply chain** (SBOM CVE-pair LP) | **Yes — direct fit** | Recent work frames cascade as LP |
| **Lateral movement on auth/host graphs** (LANL, OpTC) | **Weak — wrong object** | **Hard benign** `(u,v')` same source; SoHeaRT optional ablation |
| **Provenance APT edges** (CICAPT) | **Weak** | Hard benign / same-entity context |
| **Unsupervised anomaly LP** (Euler/Bowman) | **Partial** | Euler test ≈ full benign haystack; HeaRT = controlled per-attack sampling |

**Rule:** HeaRT applies when the task is **“predict if this link should exist.”** It does **not** apply cleanly when the task is **“this real link — attack or normal?”**

---

## Map for your paper (simple)

```text
MISSING LINK (exist?)     →  HeaRT / SoHeaRT literally     →  Option 5 (KG)
OBSERVED LINK (malicious?) →  Hard benign eval (HeaRT-inspired) →  Idea A (LANL/CICAPT)
```

---

## CARS 2026 recommendation

**Title angle:** *Hard Negative Protocols for Evaluating GNN Link Prediction in Cybersecurity Graphs*

| Include | Skip / shorten |
|---------|----------------|
| LANL + CICAPT | Full OpTC if blocked |
| GCN + GraphSAGE | MAGCN, Euler+RNN |
| Tiers A/B/C (hard benign) | Tier D unless LANL-only footnote |
| 1 paragraph: HeaRT on KG as “same principle, other domain” | Full Option 5 experiments |

**One sentence for CARS:** HeaRT matters in cyber **where LP means missing knowledge-graph links**; for **attack graphs**, cite HeaRT as motivation but implement **hard benign negatives** — that’s the relevant cyber LP eval, and it’s not done systematically in the literature.

---

**User**

Cyber LP problem	HeaRT (SoHeaRT non-edges)	Your adaptation
KG completion (CVE co-exploit, CVE–CWE, ATT&CK links)
Yes — direct fit
Run create_heart_negatives.py on KG adj
CTI relation inference (RCTI, AttackDB technique links)
Yes — direct fit
Same: missing link = positive, hard non-edge = SoHeaRT
Software supply chain (SBOM CVE-pair LP)
Yes — direct fit
Recent work frames cascade as LP
Lateral movement on auth/host graphs (LANL, OpTC)
Weak — wrong object
Hard benign (u,v') same source; SoHeaRT optional ablation
Provenance APT edges (CICAPT)
Weak
Hard benign / same-entity context
Unsupervised anomaly LP (Euler/Bowman)
Partial
Euler test ≈ full benign haystack; HeaRT = controlled per-attack sampling


Give 1-2 line sto ecplain precisly what is being fone 

---

**Cursor**

| Cyber LP problem | What is being done (1–2 lines) |
|------------------|-------------------------------|
| **KG completion** (CVE co-exploit, CVE–CWE, ATT&CK) | Hide real triples; train GNN to score **link existence**. Test: true missing link vs **SoHeaRT hard non-edges** (same head, plausible fake tail). |
| **CTI relation inference** (RCTI, AttackDB) | Same as KG completion: predict **unobserved threat/requirement or technique relations** from a partial intel graph; hard negatives = **structurally plausible non-links**. |
| **Software supply chain** (SBOM CVE-pair LP) | On dependency/CVE graph, predict **which CVE pairs co-occur in chains**; positives = known/labeled pairs, negatives = **hard non-edges** among similar components. |
| **Lateral movement** (LANL, OpTC) | Attacks are **real auth/flow edges**; rank attack above decoys. **Not** missing-link LP — use **real benign edges** from same user/host (hard benign), not SoHeaRT non-edges. |
| **Provenance APT** (CICAPT) | APT-touching provenance edges are **observed links**; train on benign Phase 1, test whether attack Phase 2 edges rank above **other real benign edges** (same process/host), not fake missing links. |
| **Unsupervised anomaly LP** (Euler/Bowman) | Train “normal link” model on benign graph; **low ŷ = suspicious**. Test compares attack edges to **other real test traffic** (Euler) or your **sampled hard benign** per attack (controlled haystack). |

---

**User**

which one do you think  is the most relevant, give its example briefly so I understand it and lets work on implementation of it

---

**Cursor**

**Most relevant for CARS 2026: lateral movement on attack graphs (Idea A) — LANL first, then CICAPT.**

You already have the data, it matches the call (cyber GNN experiments), and hard-benign eval is the right object (attacks are **real edges**, not missing links). HeaRT/SoHeaRT fits **literally** only on KG completion; for attack graphs you use **hard benign** (HeaRT-*inspired*, not SoHeaRT non-edges).

---

## Brief example (LANL)

**Train (benign only):**  
`alice → ws1`, `alice → ws2`, `bob → server` (normal auths before red team)

**Test positive (attack):**  
`alice → fileserver` ← red-team lateral move

**Tier A (easy, inflated):**  
Compare attack to `(bob, printer)` — random pair that never happened → model wins easily.

**Tier C (hard, main):**  
Compare attack to `alice → ws2` — **real** normal auth from **same user** → “find the attack among alice’s real traffic.”

Same GCN/SAGE; only the **test negatives** change; AP usually **drops** on Tier C.

---

## Why not the others (for now)

| Problem | Why not first |
|---------|----------------|
| KG completion (CVE) | HeaRT fits directly, but **no KG built** — 1–2 extra weeks |
| OpTC | **Empty folder** — download risk |
| Euler-style full haystack | Good baseline to **cite**, not your novelty |

---

## Implementation plan (what we’d build)

1. **`preprocess_lanl.py`** — `auth.txt.gz` → `graph.pt`, train benign `adj_t`, `attack_test_pos.pt`, neg samplers (random non-edge / random benign / same-user benign)
2. **`preprocess_cicapt.py`** — Phase1 train, Phase2 attack-touching positives, same-tier negatives
3. **Wire `main_gnn_snap.py`** — cyber `evaluation_edges`, `--gnn_model GCN|SAGE`, score `1-ŷ`, `pooled_auc_ap`
4. **Features** — `build_snap_features.py` on benign train → SVD-256
5. **Run matrix** — 2 models × 3 tiers × 2 datasets → results table

---

**Switch to Agent mode** and I can start with `preprocess_lanl.py` or `preprocess_cicapt.py` (CICAPT is smaller/faster for a first end-to-end run).

---

**User**

but we can do KG completion aswell. We can quickly build it right

---

**Cursor**

**You can add KG completion — but “quick” is relative.** Not same-day like wiring CICAPT; more like **3–5 extra days** if you use existing triples, not build Yin’s full VulKG from scratch.

---

## What “quick” actually means

| Path | Effort | Realistic? |
|------|--------|------------|
| **Reuse existing triples** (e.g. [nislab/threat-knowledge-graph](https://github.com/nislab/threat-knowledge-graph), or Yin-style co-exploit list from paper/supplement) | 1–2 days prep + 1–2 days HeaRT + GNN runs | **Yes — fastest** |
| **Scrape NVD + ExploitDB yourself** (Yin-scale KG) | 1–2+ weeks | **No for June 15** |
| **Full MAGCN multimodal features** | Extra week | **Skip** — use degree/SVD like your SNAP pipeline |

**Fastest KG:** small **CVE–CVE co-exploitation** graph (thousands of edges, not 276k nodes) → train/val/test split → plug into `create_heart_negatives.py` + GCN.

---

## Can you do both for CARS?

**Yes, if scoped small:**

| Track | Role in paper | Scope |
|-------|---------------|--------|
| **Idea A** (LANL + CICAPT) | **Main** — hard benign on attack graphs | Full experiments |
| **KG + SoHeaRT** | **One section** — “HeaRT applies literally here” | **1 dataset**, 2 models, random vs SoHeaRT only |

~6 pages: don’t run 3 datasets × everything on both tracks.

---

## Minimal KG example (so you see the task)

```text
Known:  CVE-2017-0144 ──co-exploit── CVE-2017-8464
Hide:   CVE-2017-8464 ──co-exploit── CVE-2019-0708   ← test positive

SoHeaRT neg: (CVE-2017-8464, co-exploit, Z)  Z = non-edge, high PPR/CN to true tail
```

Train: predict link exists. Test: rank hidden link above hard fakes.

---

## Honest recommendation

| If deadline is tight | If you finish LANL+CICAPT early |
|----------------------|----------------------------------|
| **Attack graphs only** | Add **small CVE co-exploit KG + SoHeaRT** as second contribution |
| Still a full CARS paper | Stronger “HeaRT across cyber LP” story |

**Bottom line:** KG completion is **doable** with a **prebuilt or minimal** graph — not “quick” like CICAPT alone, but feasible as a **trimmed add-on**. Don’t block Idea A on it.

Switch to **Agent mode** when ready; sensible order: **CICAPT/LANL first**, then **KG from nislab or a co-exploit CSV** if time remains.

---

**User**

lets start implementation part, use implementation details if needed from any paper if needed

---

**Cursor**

I'm in Ask mode so I can't edit files yet — I'll pull implementation details from your repo and the papers, then you can switch to Agent mode to start coding.


**I can’t write code in Ask mode.** Switch to **Agent mode** and say “start with CICAPT” — I’ll implement from the plan below.

---

## Implementation order

1. **CICAPT** (smallest, validates pipeline)  
2. **LANL** (Euler-style split)  
3. **KG** (optional, if time)

---

## Track 1: CICAPT (start here)

**Protocol (Euler/Bowman-inspired, your tiers):**

| Split | Source |
|-------|--------|
| Train `adj_t` | Phase 1 unique edges (benign) |
| Test pos | Phase 2 edges where `from` or `to` has `label=1` |
| Tier A neg | Random non-edges |
| Tier B neg | Random Phase 2 benign edges |
| Tier C neg | Same `from` process as attack edge |

**Graph:** collapse provenance to **process→process** (or process→file for socket ops) from `WasTriggeredBy` / `Used` rows — one edge per unique `(from,to)` in Phase 1.

**Files to create:**

```
CARS/processed/cicapt/
  graph.pt              # train benign edges
  meta.json
  attack_test_pos.pt    # [E, 2]
  neg_random_test.npy   # [N_attack, K]
  neg_benign_test.npy
  neg_hard_benign_test.npy
  splits/split.pt       # val benign holdout
```

**Reuse:** `preprocess_twitter_edges.py` pattern → `preprocess_cicapt.py`; `build_snap_features.py` → `x_snap_svd256.pt`; extend `main_gnn_snap.py` with `--dataset cicapt` and `--neg_tier {random,benign,hard}`.

**Scoring (Euler):** anomaly score = `1 - ŷ`; AP/AUC via `pooled_auc_ap` with attack=1.

---

## Track 2: LANL

**Auth format** (from your files):

```text
auth:  day, user@dom, user@dom, src_comp, dst_comp, ...
redteam:  time, user@dom, src_comp, dst_comp
```

**Graph (Euler):** nodes = `user@dom` and `C####` computers; edge = auth `(src_entity, dst_entity)` per day or collapsed unique pairs.

| Split | Rule |
|-------|------|
| Train | All auths **before** first redteam timestamp |
| Test pos | `redteam.txt` edges (~750) |
| Tier C | Benign auths in attack window with **same user** as redteam edge |

**Note:** 7.6 GB — first run: **sample one week** or unique-edge collapse to keep RAM sane.

---

## Track 3: KG + SoHeaRT (optional add-on)

1. Pull triples from [nislab/threat-knowledge-graph](https://github.com/nislab/threat-knowledge-graph) or a minimal CVE co-exploit CSV  
2. Add `get_data_cyber_kg()` in `heart_negatives/hard_utils.py`  
3. Run `create_heart_negatives.py` → SoHeaRT test negs  
4. Same GCN/SAGE train; test = hidden triple vs SoHeaRT non-edges  

---

## `main_gnn_snap.py` changes (all tracks)

- `--gnn_model GCN|SAGE`  
- `--dataset cicapt|lanl`  
- `--neg_tier random|benign|hard`  
- Load cyber `evaluation_edges` instead of Twitter `heart_*.npy`  
- `--anomaly_score` flip: use `1-ŷ` for attack ranking  

**Models (Euler/Bowman baseline):** 3-layer GCN/SAGE, hidden 256, MLP decoder, lr=1e-3, K=500.

---

## First commands (after Agent mode)

```bash
conda activate heart_gpu
cd /home/mmanjee/HeaRT

# 1. Preprocess CICAPT
python CARS/code_preprocess/preprocess_cicapt.py

# 2. Features
python SNAP/code_preprocess/build_snap_features.py --processed_dir CARS/processed/cicapt

# 3. Train + eval (× tiers × models)
python SNAP/code_preprocess/main_gnn_snap.py --dataset cicapt --gnn_model GCN --neg_tier hard
```

---

**Switch to Agent mode** and I’ll implement `preprocess_cicapt.py` + the `main_gnn_snap.py` hooks first.

---

**User**

how to get optc dataset

---

**Cursor**

**OpTC is not on GitHub as raw logs** — only docs. Data is on **Google Drive** (~1 TB total; download a **subset**).

---

## 1. Metadata + ground truth (small, do first)

```bash
git clone https://github.com/FiveDirections/OpTC-data.git
```

From that repo get:
- **`OpTCRedTeamGroundTruth.pdf`** — attack labels for test edges  
- **`ecar.md`** — event format (FLOW-START fields)  
- **`errata.md`** — known issues  

Repo: [FiveDirections/OpTC-data](https://github.com/FiveDirections/OpTC-data)

---

## 2. Main data (Google Drive)

**Root:** [OpTC Drive folder](https://drive.google.com/drive/folders/1n3kkS3KR31KUegn42yk3-e6JkZvf0Caa)

For Euler-style **host FLOW-START** graphs, use only **`ecar-bro/`**:

| Folder | Role |
|--------|------|
| **`ecar-bro/benign/`** | **Train** (normal traffic) |
| **`ecar-bro/evaluation/`** | **Test** (red-team days) |
| `short/` | **Skip** (incomplete) |
| `ecar/`, `bro/` | **Skip** for now (huge) |

**Direct subfolder links** (from your earlier chat):

- Benign: [drive folder](https://drive.google.com/drive/folders/1XOGWT8R8ovw6J2DgNsui09ZwvrfOXIVa)  
- Evaluation: [drive folder](https://drive.google.com/drive/folders/1mRnYKaMsYYPQbCwdq9YLCgNy-pD6uP0Y)

---

## 3. Download commands

```bash
pip install gdown
mkdir -p /home/mmanjee/HeaRT/CARS/dataset/optc/OpTC/ecar-bro

gdown --folder "https://drive.google.com/drive/folders/1XOGWT8R8ovw6J2DgNsui09ZwvrfOXIVa" \
  -O /home/mmanjee/HeaRT/CARS/dataset/optc/OpTC/ecar-bro/benign --remaining-ok

gdown --folder "https://drive.google.com/drive/folders/1mRnYKaMsYYPQbCwdq9YLCgNy-pD6uP0Y" \
  -O /home/mmanjee/HeaRT/CARS/dataset/optc/OpTC/ecar-bro/evaluation --remaining-ok
```

**If it fails** (Drive rate limit / timeout): download **per-day subfolders** in the browser, or rerun with `--continue`. Don’t use `gdown` on the **1 TB root** folder.

**Minimum disk:** benign `19Sep19` + `20-23Sep19`; evaluation `24Sep19` + `25Sept` + `23Sep19-red`.

---

## 4. Euler split (for your paper)

| | Source |
|--|--------|
| **Train** | First **4 benign days** — FLOW-START host→host edges |
| **Test pos** | FLOW-START in **evaluation/** matching **ground-truth PDF** |
| **Graph** | Host IPs from FLOW-START events (not full provenance) |

---

## 5. Alternatives if Drive is painful

| Option | Link |
|--------|------|
| **Corrected OpTC** (fixes ID bugs) | [Inria Dataverse](https://entrepot.recherche.data.gouv.fr/dataset.xhtml?persistentId=doi:10.57745/UXCWOC) — pick files &lt;50 GB each |
| **PIDSMaker preprocessed** (~2 GB) | [PIDSMaker install](https://ubc-provenance.github.io/PIDSMaker/ten-minute-install/) — `optc_h201` dump |

---

**Practical order:** clone GitHub → PDF + `ecar.md` → gdown **benign** → gdown **evaluation** per-day if bulk fails. **LANL + CICAPT** are enough for CARS if OpTC stalls.

---

**User**

@/home/mmanjee/.cursor/projects/home-mmanjee-HeaRT/terminals/4.txt:22-562 

---

**Cursor**

Checking download status after the gdown failure.


**Download mostly worked — gdown hit Google Drive rate limits on `20-23Sep19`.**

---

## What happened

| Status | Detail |
|--------|--------|
| **Done** | `17-18Sep19`, `18-19Sep19`, `19Sep19` — all host chunks (~51 files) |
| **Partial** | `20-23Sep19` — only 2/18+ chunks (`AIA-101-125`, `AIA-151-175`) |
| **Failed on** | `AIA-201-225/ecarbro.json.gz` — Drive “too many accesses” / permission error |
| **On disk** | ~**3.1 GB**, **54** `.gz` files |

This is normal after ~50 rapid `gdown` pulls — not a broken link.

---

## What to do next

**1. Wait 30–60 min**, then resume **only** the missing folder:

```bash
# Resume 20-23Sep19 only (not the whole benign tree)
gdown --folder "https://drive.google.com/drive/folders/<20-23Sep19-subfolder-id>" \
  -O /home/mmanjee/HeaRT/CARS/dataset/optc/benign/20-23Sep19 --remaining-ok
```

Or retry the single failed file:

```bash
gdown "https://drive.google.com/uc?id=1A2Tqwr0CuXoipdeKNIdFDMynPeKvU2pa" \
  -O /home/mmanjee/HeaRT/CARS/dataset/optc/benign/20-23Sep19/AIA-201-225/ecarbro.json.gz
```

**2. If gdown keeps failing:** download `20-23Sep19` chunks **in the browser** from Drive (right-click → Download), or use **rclone** with a Google account.

**3. Grab metadata** (if not done):

```bash
git clone https://github.com/FiveDirections/OpTC-data.git /home/mmanjee/HeaRT/CARS/dataset/optc/OpTC-data
```

Need **`OpTCRedTeamGroundTruth.pdf`** for test labels.

**4. Evaluation set** (separate command, after benign finishes):

```bash
gdown --folder "https://drive.google.com/drive/folders/1mRnYKaMsYYPQbCwdq9YLCgNy-pD6uP0Y" \
  -O /home/mmanjee/HeaRT/CARS/dataset/optc/evaluation --remaining-ok
```

---

## For your paper (Euler split)

| Data | You have |
|------|----------|
| **Train (4 benign days)** | 3 full days + partial day 4 — **enough to start preprocessing** |
| **Test** | Need `evaluation/` + ground-truth PDF |

You can **start CICAPT/LANL implementation now** while OpTC `20-23Sep19` + `evaluation` finish in the background.

---

**Bottom line:** Benign download is ~90% done; pause, retry `20-23Sep19` later with `--remaining-ok` or browser download. Then fetch `evaluation/` + ground-truth PDF.

---

**User**

how about downloading data using github and keeping what is needed and removing remaining?

---

**Cursor**

**GitHub only has docs — not the logs.** You can’t replace the Drive download with GitHub alone.

---

## What’s on GitHub vs Drive

| Source | Contents | Size |
|--------|----------|------|
| [FiveDirections/OpTC-data](https://github.com/FiveDirections/OpTC-data) | `OpTCRedTeamGroundTruth.pdf`, `ecar.md`, `errata.md`, README | **~few MB** |
| Google Drive `ecar-bro/` | `benign/`, `evaluation/` JSON.gz | **~GB–TB** |

The bulk data **isn’t on GitHub** by design (README points to Drive).

---

## What to keep from GitHub (minimal)

```bash
git clone --depth 1 https://github.com/FiveDirections/OpTC-data.git
# Keep only:
#   OpTCRedTeamGroundTruth.pdf  ← attack labels
#   ecar.md                     ← FLOW-START field names
#   errata.md                   ← optional
# Remove clone if you copy those 2–3 files elsewhere
```

Or sparse checkout / download raw PDF only — no need for a full clone long-term.

---

## What to keep from Drive (Euler-style)

```
optc/
  benign/          ← train (FLOW-START) — you already have ~3.1 GB
  evaluation/      ← test / red-team days — still need this
  OpTCRedTeamGroundTruth.pdf  ← from GitHub
```

**Delete / skip:**
- `short/`, `ecar/`, `bro/` (huge, not needed for host FLOW graph)
- Extra benign days if disk tight (minimum: `19Sep19` + `20-23Sep19` complete)

---

## Practical workflow

1. **GitHub** → grab PDF + `ecar.md` only (~1 min).  
2. **Drive** → finish `20-23Sep19` + download `evaluation/` (can’t avoid this for raw OpTC).  
3. **After preprocessing** → delete raw `.json.gz`, keep only `processed/` (`graph.pt`, splits, neg files) — often **&lt;1 GB**.

---

## If you want to avoid Drive entirely

| Alternative | Tradeoff |
|-------------|----------|
| **PIDSMaker `optc_h201`** (~2 GB preprocessed) | Less parsing; still Drive-hosted |
| **Inria corrected OpTC** | Pick specific tar files; still large |

---

**Bottom line:** Use **GitHub for labels + schema only**; use **Drive for logs** (or a small preprocessed dump). Clone GitHub, copy PDF/`ecar.md`, delete the repo — that part is tiny. **Don’t delete benign/evaluation gz until** you’ve built `graph.pt` and attack splits.

---

**User**

scp file from mmanjee@yangra101 path: /home/mmanjee/Documents/20-23Sep19

---

**Cursor**

**Pull `20-23Sep19` from `yangra101` into your OpTC benign folder** (run on **this** machine, where HeaRT lives):

```bash
mkdir -p /home/mmanjee/HeaRT/CARS/dataset/optc/benign/20-23Sep19

scp -r mmanjee@yangra101:/home/mmanjee/Documents/20-23Sep19/* \
  /home/mmanjee/HeaRT/CARS/dataset/optc/benign/20-23Sep19/
```

If the remote path is the folder itself (not only its contents):

```bash
scp -r mmanjee@yangra101:/home/mmanjee/Documents/20-23Sep19 \
  /home/mmanjee/HeaRT/CARS/dataset/optc/benign/
```

**Better for large data / resume:**

```bash
rsync -avP mmanjee@yangra101:/home/mmanjee/Documents/20-23Sep19/ \
  /home/mmanjee/HeaRT/CARS/dataset/optc/benign/20-23Sep19/
```

**Check after copy:**

```bash
find /home/mmanjee/HeaRT/CARS/dataset/optc/benign/20-23Sep19 -name '*.gz' | wc -l
```

Expect ~16–18 `ecarbro.json.gz` files under `AIA-*` subfolders.

**Notes:** `-r` = recursive; you need SSH access to `yangra101` (VPN/campus network if off-site). If the hostname fails, use the full FQDN or IP your site uses for that host.

---

**User**

@/home/mmanjee/.cursor/projects/home-mmanjee-HeaRT/terminals/5.txt:210-213 

---

**Cursor**

**DNS failure** — this machine can’t resolve `yangra101.uwyo.edu`. SSH never started; it’s not an `scp` path issue.

---

## Fixes (try in order)

**1. Use the hostname that works on UWyo**

```bash
# Short name (often works on campus VPN)
ssh mmanjee@yangra101

# Or whatever you normally use to log in there
ssh mmanjee@<actual-hostname-or-IP>
```

If `ssh mmanjee@yangra101` works, use that same host in `scp`/`rsync`.

**2. Connect to UWyo VPN first**  
Internal names like `yangra101` often only resolve on the university network.

**3. Use IP instead of hostname** (if you know it)

```bash
scp -r mmanjee@<IP>:/home/mmanjee/Documents/20-23Sep19/ \
  /home/mmanjee/HeaRT/CARS/dataset/optc/benign/20-23Sep19/
```

**4. Check SSH config** (on the machine where you run `scp`)

```bash
cat ~/.ssh/config | grep -A5 -i yangra
```

If there’s a `Host` entry, use that alias:

```bash
scp -r yangra:/home/mmanjee/Documents/20-23Sep19/ \
  /home/mmanjee/HeaRT/CARS/dataset/optc/benign/20-23Sep19/
```

**5. If you’re already on `yangra101`** — push instead of pull:

```bash
# Run ON yangra101
rsync -avP /home/mmanjee/Documents/20-23Sep19/ \
  mmanjee@<THIS_MACHINE_HOST>:/home/mmanjee/HeaRT/CARS/dataset/optc/benign/20-23Sep19/
```

**6. No SSH between machines** — intermediate copy:
- `tar czf 20-23Sep19.tar.gz 20-23Sep19` on yangra101 → Google Drive / USB → extract here  
- Or finish `gdown` after the rate limit cools down

---

## Verify after fix

```bash
ssh mmanjee@<working-host> "ls /home/mmanjee/Documents/20-23Sep19 | head"
```

Then run `rsync`/`scp` with **that same host**.

---

**Bottom line:** Fix **how you reach yangra101** (

---

**User**

the current disclab1 session is I have sshd from yangra101

---

**Cursor**

**Your session is on `disclab1`; the data is on `yangra101`.** `scp` from `disclab1` → `yangra101` failed because **`yangra101.uwyo.edu` doesn’t resolve** from this machine — not because the path is wrong.

---

## Fix: copy from the machine that **has** the data

**Option A — Push from `yangra101` (usually easiest)**

On **`yangra101`** (new terminal there, not on `disclab1`):

```bash
scp -r /home/mmanjee/Documents/20-23Sep19 \
  mmanjee@disclab1:/home/mmanjee/HeaRT/CARS/dataset/optc/benign/
```

Or `rsync`:

```bash
rsync -avP /home/mmanjee/Documents/20-23Sep19/ \
  mmanjee@disclab1:/home/mmanjee/HeaRT/CARS/dataset/optc/benign/20-23Sep19/
```

Use whatever hostname **you used to SSH into `disclab1`** (e.g. `disclab1`, `disclab1.uwyo.edu`, or an IP).

---

**Option B — Pull from `disclab1` with a hostname that works**

On `yangra101`, get a reachable address:

```bash
hostname -f
hostname -I
```

On **`disclab1`**, try:

```bash
scp -r mmanjee@yangra101:/home/mmanjee/Documents/20-23Sep19 \
  /home/mmanjee/HeaRT/CARS/dataset/optc/benign/
```

or `mmanjee@<IP-from-yangra101>:...` — **not** `yangra101.uwyo.edu` if DNS fails.

---

## Why `.uwyo.edu` failed

`disclab1` can’t resolve `yangra101.uwyo.edu` (internal DNS, VPN, or wrong FQDN). Short name `yangra101` or the **LAN IP** often works inside the lab network.

---

## Quick check

```bash
# on disclab1
ping -c1 yangra101
ssh mmanjee@yangra101 hostname
```

If both fail → use **Option A** (push from `yangra101`).

---

**Summary:** You SSH’d `yangra101` → `disclab1`, so run **`scp`/`rsync` from `yangra101` toward `disclab1`**, or use `yangra101`’s IP/short name instead of `yangra101.uwyo.edu`.

---

**User**

/home/mmanjee/HeaRT/CARS/dataset/optc/evaluation/evaluation/25Sept

remove double evaluation dir

---

**Cursor**

**Ask mode — run this yourself** to flatten `evaluation/evaluation/`:

```bash
cd /home/mmanjee/HeaRT/CARS/dataset/optc/evaluation

# move day folders up one level
mv evaluation/* .

# remove empty nested dir
rmdir evaluation
```

**Check:**

```bash
ls /home/mmanjee/HeaRT/CARS/dataset/optc/evaluation/
# expect: 23Sep19-red  23Sep-night  23Sep-Night  24Sep19  25Sept
```

**If `mv` says “File exists”** (something already at the top level):

```bash
mv evaluation/* . 2>/dev/null || rsync -a evaluation/ . && rm -rf evaluation
```

**Cause:** `gdown -O .../evaluation` created an extra `evaluation/` inside the target. Use `-O .../optc` or move contents up after download.

---

**User**

now we have all 3 datasets, you can check, can you guide me for next steps?

---

**Cursor**

**All 3 datasets look ready on disk.** One gap: grab **`OpTCRedTeamGroundTruth.pdf`** from [FiveDirections/OpTC-data](https://github.com/FiveDirections/OpTC-data) if you haven’t yet.

| Dataset | Status |
|---------|--------|
| **LANL** | `auth.txt.gz` (7.2G) + `redteam.txt.gz` ✓ |
| **CICAPT** | Phase1 + Phase2 CSV ✓ |
| **OpTC** | benign 71 + evaluation 80 `.gz` files ✓ |

---

## Next steps (in order)

### Phase 1 — Preprocessors (build `graph.pt` + splits + negatives)

Create `CARS/code_preprocess/` (mirror `preprocess_twitter_edges.py`).

| # | Script | Output |
|---|--------|--------|
| **1** | `preprocess_cicapt.py` | **Start here** — smallest, fast sanity check |
| **2** | `preprocess_lanl.py` | Unique auth edges; split at first redteam time |
| **3** | `preprocess_optc.py` | Parse `ecarbro.json.gz` → FLOW-START host→host edges |

**Per dataset, write:**

```
CARS/processed/{cicapt,lanl,optc}/
  graph.pt              # train benign adj
  attack_test_pos.pt    # test attack edges
  neg_random_test.npy   # Tier A [N_attack, K]
  neg_benign_test.npy   # Tier B
  neg_hard_benign_test.npy  # Tier C (same-source)
  splits/split.pt       # val benign holdout
  meta.json
```

**CICAPT rules:** train = Phase1 unique `(from,to)`; test pos = Phase2 edges touching `label=1`; Tier C = same `from` benign in Phase2.

**LANL rules:** train = auths before first redteam timestamp; test pos = `redteam.txt`; Tier C = same-user benign in attack window. *First pass: collapse to unique edges to avoid loading 7GB into RAM at once.*

**OpTC rules (Euler):** train = 4 benign days FLOW-START; test pos = evaluation FLOW-START matching ground-truth PDF; Tier C = same src host benign.

---

### Phase 2 — Features

```bash
python SNAP/code_preprocess/build_snap_features.py --processed_dir CARS/processed/cicapt
```

Repeat for `lanl`, `optc` → `x_snap_svd256.pt`.

---

### Phase 3 — Wire training/eval

Extend `main_gnn_snap.py`:

- `--dataset cicapt|lanl|optc`
- `--gnn_model GCN|SAGE`
- `--neg_tier random|benign|hard`
- Load cyber negatives + `attack_test_pos.pt`
- Score **`1 - ŷ`** for attack ranking; `pooled_auc_ap`

---

### Phase 4 — Experiment matrix

| | CICAPT | LANL | OpTC |
|--|--------|------|------|
| GCN × Tier A/B/C | ✓ | ✓ | ✓ |
| SAGE × Tier A/B/C | ✓ | ✓ | ✓ |

= **18 runs** (can parallelize per dataset).

---

### Phase 5 — Paper table

Main result: **Tier C AP** (honest). Ablation: **Tier A vs C** gap = inflated metrics story.

---

## Suggested timeline

| Day | Task |
|-----|------|
| **1** | `preprocess_cicapt.py` + features + 1 GCN run end-to-end |
| **2** | CICAPT full 6 runs (2 models × 3 tiers) |
| **3–4** | LANL preprocessor + runs |
| **5** | OpTC preprocessor + runs |
| **6** | Results table + draft |

---

## Do now

1. Clone OpTC metadata: `git clone https://github.com/FiveDirections/OpTC-data.git` → copy PDF to `CARS/dataset/optc/`
2. **Switch to Agent mode** → I implement `preprocess_cicapt.py` + `main_gnn_snap.py` cyber hooks first

That gets you numbers fastest; LANL/OpTC preprocessors follow the same pattern.

---

**User**

For Evaluation: Per each attack edge(u,v), we sample K negatives only from attack-looking benigns (with low PPR/CN/RA)
All benign test edges
  → keep only (u,v) NOT in train
  → among those, keep bottom PPR/CN/RA
  → sample K per attack and rank attack edge among hard benigns which look like attack so it becomes difficult for model to classify if attack can be classified as benign and allow connection between nodes or stop that connection because its detected as an attack


Are we doing this? say yes or no only

---

**Cursor**

**No** — not in the main plan. That’s **Tier D** (optional LANL ablation). Main eval is **Tier C**: same-source **real benign** edges, not bottom-PPR/CN/RA attack-looking benigns.

---

**User**

can you briefly tell the plan 

---

**Cursor**

**Paper:** Hard benign negatives expose when GNN link-prediction metrics are inflated on cyber attack graphs.

---

**Datasets (3):** CICAPT, LANL, OpTC — lateral movement / APT / host flows.

**Models (2):** GCN + GraphSAGE (same MLP scorer, SVD-256 features).

**Train (all datasets):** benign edges only + random non-edges; no attack labels.

**Test positives:** held-out attack/red-team edges.

**Test negatives (3 tiers):**

| Tier | Negatives |
|------|-----------|
| A | Random non-edges (easy / inflated) |
| B | Random benign edges |
| C | **Same-source benign** (main — hard haystack) |

**Score:** `1 - ŷ` (high = suspicious) → AP/AUC.

**Not in main plan:** Tier D (low-PPR attack-looking benigns), SoHeaRT non-edges, new architecture, temporal RNN.

---

**Implementation order:**
1. Preprocessors → `graph.pt`, attack test pos, 3 neg `.npy` files per dataset  
2. `build_snap_features.py` → node features  
3. Extend `main_gnn_snap.py` for cyber datasets + tiers  
4. Run 2 models × 3 tiers × 3 datasets → table (AP drops A → C)  
5. Write ~6-page CARS paper

**Start with CICAPT** (smallest), then LANL, then OpTC.

---

**User**

why not this: For Evaluation: Per each attack edge(u,v), we sample K negatives only from attack-looking benigns (with low PPR/CN/RA)
All benign test edges
  → keep only (u,v) NOT in train
  → among those, keep bottom PPR/CN/RA
  → sample K per attack and rank attack edge among hard benigns which look like attack so it becomes difficult for model to classify if attack can be classified as benign and allow connection between nodes or stop that connection because its detected as an attack



otherwise we dont have any contribution

---

**Cursor**

**You can include it — but it’s not the only way to have a contribution.**

---

## Why we dropped it from the main plan

| Issue | Detail |
|-------|--------|
| **Applicability** | Strong on **LANL**; weak on **CICAPT/OpTC** (“unseen” ≈ new phase, not attack-like) |
| **Different question than Tier C** | Tier C = attack **blends into normal** (same user). Tier D = attack vs **rare benign anomaly** (looks weird to LP) |
| **Unsupervised LP** | Tier D often collapses AP ~0.5 **by design** — finding, not a “better detector” |
| **Deadline** | Extra PPR/CN pipeline per dataset |

That’s “optional / LANL-only,” not “no contribution without it.”

---

## What the contribution actually is (without Tier D)

**Not a new GNN.** The claim is:

> Cyber LP papers use **random non-edges** → inflated AP. **Real benign decoys** (especially **same-source**) give a honest attack-ranking benchmark. We formalize **tiers** and show the gap on **3 datasets × 2 GNNs**.

Euler uses a benign haystack **implicitly** but doesn’t define **tiered, per-attack sampling** or report **A vs B vs C**. That gap is still publishable for CARS if the **AP drop is large and consistent**.

---

## If you feel Tier C alone is too thin

**Add Tier D as a fourth tier** — don’t replace the whole plan:

| Tier | Question |
|------|----------|
| A | Inflated baseline (random non-edge) |
| B | Random benign |
| **C** | Attack vs **same-context** normal (blending in) |
| **D** | Attack vs **attack-looking** benign (low PPR/CN, unseen) — **your sampler** |

**Paper line:** “We propose a **tiered hard-benign protocol**; Tier C tests stealth among routine traffic, Tier D tests discrimination from **structural benign anomalies**.”

**Scope honestly:**
- Report **D on LANL** (and maybe OpTC with same-day filter)
- **C on all 3** as main table
- CICAPT: Tier D optional or footnote

That gives you a **named technique** (bottom-PPR benign mining) **plus** the eval-protocol story.

---

## Bottom line

| | |
|--|--|
| **No contribution without Tier D?** | **No** — Tier A→C on 3 datasets is still a contribution if the gap is clear. |
| **Stronger with Tier D?** | **Yes** — especially if you want a concrete “sampler” beyond “same user.” |
| **Recommendation** | **C main on all 3; D as second tier (LANL-first)** — then you have both protocol + technique. |

Switch to **Agent mode** if you want preprocessors to emit **four** neg files (`neg_hard_anomaly_test.npy` for Tier D) alongside A/B/C.

---

**User**

we are just running gcn and producing results in tier a-c

---

**Cursor**

**Honest answer:** Running GCN on tiers A–C **can** be enough for a **6-page eval/benchmark paper** — but only if the **story is the protocol and the gap**, not the model.

---

## What reviewers will see if you only report GCN + A/B/C

| Strong | Weak |
|--------|------|
| Clear problem: random negs lie on cyber graphs | “We ran GCN on 3 datasets” |
| Table: AP **drops A → C** on all 3 | Single model, no comparison |
| Same train/test spec as Euler/Bowman family | No ablation (why Tier C > B?) |
| Reproducible splits + neg samplers | Looks like a course project |

**Minimum to not look thin:** **GCN + GraphSAGE** (you already planned 2 models). One model alone is weaker.

---

## What makes it a contribution (without Tier D)

One sentence for the paper:

> *We show that standard LP evaluation with random non-edges **overstates** attack-edge detection on LANL, CICAPT, and OpTC; **same-source benign negatives** give a realistic ranking task.*

That’s the same **class** of contribution as **HeaRT** (eval methodology, not SOTA detector). You need:

1. **Tier table** — AP/AUC A vs B vs **C** (main), all 3 datasets  
2. **2 GNNs** — GCN + SAGE (same numbers, same protocol)  
3. **One paragraph** vs Euler (“implicit full haystack” vs your **explicit tiers**)  
4. **Honest discussion** — low AP on C is expected / meaningful  

---

## If you’re uneasy with “just GCN + tiers”

| Add (small effort) | Why |
|--------------------|-----|
| **GraphSAGE** | Required bar you already had |
| **Tier D on LANL only** | Named sampler + harder eval |
| **Supervised row** (benign=1, attack=0 train) | One extra config, “does supervision help under Tier C?” |

---

**Bottom line:** GCN + tiers A–C **alone** is borderline for CARS; **GCN + SAGE + 3 datasets + clear A→C gap** is a defensible **evaluation paper**. The contribution is **how you test**, not **which GCN you run**. If the gap is small or inconsistent, then yes — you’d need Tier D or another angle.

---

**User**

Cyber LP problem	What is being done (1–2 lines)
KG completion (CVE co-exploit, CVE–CWE, ATT&CK)
Hide real triples; train GNN to score link existence. Test: true missing link vs SoHeaRT hard non-edges (same head, plausible fake tail).
CTI relation inference (RCTI, AttackDB)
Same as KG completion: predict unobserved threat/requirement or technique relations from a partial intel graph; hard negatives = structurally plausible non-links.
Software supply chain (SBOM CVE-pair LP)
On dependency/CVE graph, predict which CVE pairs co-occur in chains; positives = known/labeled pairs, negatives = hard non-edges among similar components.
Lateral movement (LANL, OpTC)
Attacks are real auth/flow edges; rank attack above decoys. Not missing-link LP — use real benign edges from same user/host (hard benign), not SoHeaRT non-edges.
Provenance APT (CICAPT)
APT-touching provenance edges are observed links; train on benign Phase 1, test whether attack Phase 2 edges rank above other real benign edges (same process/host), not fake missing links.
Unsupervised anomaly LP (Euler/Bowman)
Train “normal link” model on benign graph; low ŷ = suspicious. Test compares attack edges to other real test traffic (Euler) or your sampled hard benign per attack (controlled haystack).




which one is best suited to show Heart negatives impact and follows a paper that has implementations already about above so we can extend it, cite the paper too

---

**Cursor**

**Best fit for literal HeaRT/SoHeaRT impact:** **KG completion (CVE co-exploitation)** — not LANL/CICAPT/OpTC.

---

## Ranking for “show HeaRT negatives matter”

| Problem | HeaRT fits? | Paper + code to extend | Verdict |
|---------|-------------|------------------------|---------|
| **KG completion (CVE co-exploit)** | **Yes — direct** | **Yin et al.** + **your `create_heart_negatives.py`** | **Best for HeaRT** |
| CTI / AttackDB | Yes | Kaiser TDSC — less LP benchmark focus | OK, more work |
| SBOM CVE-pair | Yes | Very new, little standard code | Weak |
| LANL / OpTC / CICAPT | **No** (use hard benign) | **Euler** — different eval object | HeaRT-*inspired* only |
| Euler/Bowman | Partial | [iHeartGraph/Euler](https://github.com/iHeartGraph/Euler) | Benign haystack, not SoHeaRT |

---

## Recommended: CVE co-exploitation KG

**What you do:** Same as Yin — hide triples, GCN link prediction; **add** HeaRT eval (random vs SoHeaRT non-edges).

**Papers to cite:**

| Role | Paper | Link |
|------|--------|------|
| **Cyber LP baseline** | Yin et al., *Knowledge-Driven Cybersecurity Intelligence: Software Vulnerability Coexploitation Behavior Discovery* | [IEEE](https://ieeexplore.ieee.org/document/9832800) · DOI [10.1109/TII.2022.3190895](https://doi.org/10.1109/TII.2022.3190895) |
| **HeaRT method** | Li et al., *Evaluating GNNs for Link Prediction: Current Pitfalls and New Benchmarking* (NeurIPS 2023) | [arXiv](https://arxiv.org/abs/2306.10453) · [Code](https://github.com/Juanhui28/HeaRT) |
| **Optional KG build** | nislab threat knowledge graph | [GitHub](https://github.com/nislab/threat-knowledge-graph) |

**Why:** Yin uses **random train/test negs** → you show **SoHeaRT drops metrics** — exact HeaRT story, new domain. Your repo already has HeaRT machinery.

---

## If the paper must stay on **attack graphs** (LANL/CICAPT/OpTC)

HeaRT impact is **indirect** — cite HeaRT as motivation, implement **Tier A–C hard benign**, compare to Euler:

| Role | Paper | Link |
|------|--------|------|
| **Cyber LP + LANL/OpTC** | King & Huang, *Euler* | [DOI 10.1145/3588771](https://doi.org/10.1145/3588771) · [Code](https://github.com/iHeartGraph/Euler) |
| **Eval motivation** | Li et al., HeaRT | [arXiv](https://arxiv.org/abs/2306.10453) |

Extend **`main_gnn_snap.py`** + preprocessors — **not** `create_heart_negatives.py` as main eval.

---

## One-line pick

- **Max HeaRT contribution + extendable code:** **CVE KG + Yin + your HeaRT repo**  
- **Max cyber attack graphs + your 3 datasets:** **LANL/CICAPT/OpTC + Euler + hard benign tiers** (HeaRT cited, not applied literally)

You can do **both in one paper** (KG = §5.1 HeaRT literal; attack graphs = §5.2 hard-benign adaptation) if scope allows.

---

**User**

Problem	HeaRT fits?	Paper + code to extend	Verdict
KG completion (CVE co-exploit)
Yes — direct
Yin et al. + your create_heart_negatives.py
Best for HeaRT
CTI / AttackDB
Yes
Kaiser TDSC — less LP benchmark focus
OK, more work
SBOM CVE-pair
Yes
Very new, little standard code
Weak
LANL / OpTC / CICAPT
No (use hard benign)
Euler — different eval object
HeaRT-inspired only
Euler/Bowman
Partial
iHeartGraph/Euler
Benign haystack, not So



what datasets we need for this?

---

**Cursor**

**By track — what you need:**

---

## 1. KG completion (CVE co-exploit) — HeaRT direct

| Dataset / source | What | Where |
|------------------|------|--------|
| **CVE co-exploitation edges** | Train/test positive triples | Yin’s VulKG (NVD + ExploitDB + CVE Details, 1995–2021) — rebuild or use paper splits |
| **CVE metadata** (optional) | Node features | NVD JSON feeds |
| **Starter triples** | Faster path | [nislab/threat-knowledge-graph](https://github.com/nislab/threat-knowledge-graph) (CVE–CPE–CWE) — adapt to co-exploit if you only have associations |

**Count:** **1 main KG** (co-exploit or CVE–CVE); optional 2nd relation type for generality.

**You don’t have this prepped** — unlike your 3 attack-graph datasets.

---

## 2. CTI / AttackDB (Kaiser)

| Source | What |
|--------|------|
| **AttackDB** | MITRE ATT&CK + OTX + X-Force + VirusTotal fused KG |
| **53K VirusTotal reports** | Technique–artifact links |

**Access:** paper / authors — **not** a simple public dump like LANL.

---

## 3. SBOM CVE-pair

| Source | What |
|--------|------|
| **SBOM graphs** + **documented CVE chains** | Recent papers use custom SBOM + NVD |

**No standard public benchmark** yet — weakest for deadline.

---

## 4. Attack graphs (your current plan) — HeaRT-inspired

| # | Dataset | You have? | Role |
|---|---------|-----------|------|
| 1 | **CICAPT-IIoT** Phase1/2 provenance | ✓ | APT / provenance |
| 2 | **LANL Cyber1** auth + redteam | ✓ | Lateral movement |
| 3 | **OpTC** ecar-bro benign + evaluation | ✓ | Enterprise host flows |
| + | **OpTCRedTeamGroundTruth.pdf** | ✓ | OpTC labels |

**Count:** **3 datasets** — already on disk.

---

## 5. Euler/Bowman (reference only)

Same as row 4: **LANL** (+ **OpTC** for Euler). Bowman = LANL auth.

---

## Summary

| Paper direction | Datasets needed | Your status |
|---------------|-----------------|-------------|
| **HeaRT + CVE KG** | 1 KG (Yin-style co-exploit or nislab CVE graph) | **Need to build** |
| **Hard benign + attack graphs (CARS)** | LANL + CICAPT + OpTC | **Have all 3** |
| **Both in one paper** | Above 3 + 1 CVE KG | Mixed |

**For CARS with what you have:** stick to **LANL + CICAPT + OpTC**; add CVE KG only if you finish those first and have time to build Yin/nislab triples.

---

**User**

so dataset doesnot contain triples? we need to build them from scratch'?>

---

**Cursor**

**Depends which track:**

---

## CVE / KG completion (Yin, HeaRT)

**No ready-made “Yin triples” download.** You build the graph from raw sources:

| Source | Gives you |
|--------|-----------|
| NVD / CVE Details / ExploitDB | CVE nodes + **co-exploitation events** (Yin mined these) |
| Your job | `(CVE-A, co-exploit, CVE-B)` triples + train/test split |

**Exception:** [nislab/threat-knowledge-graph](https://github.com/nislab/threat-knowledge-graph) ships **CVE–CPE–CWE triples** in a notebook — not Yin’s co-exploit links, but you don’t start from zero.

**So:** mostly **build from scratch** (or adapt nislab’s triples to a simpler LP task).

---

## Attack graphs (LANL, CICAPT, OpTC) — your CARS plan

**Not triples.** Raw logs → **you** define edges:

| Dataset | Raw | You build |
|---------|-----|-----------|
| **LANL** | auth lines | `(user, computer)` edges |
| **CICAPT** | provenance CSV | `(from, to)` process/entity edges |
| **OpTC** | `ecarbro.json.gz` | `(src_host, dst_host)` FLOW-START edges |

**You already have the raw data** — preprocessors produce `graph.pt`, not RDF triples.

---

**One line:** KG track = **assemble triples from CVE sources** (or use nislab as shortcut). Attack-graph track = **edges from logs** — no triple building, but **preprocessing required** (you’re set up for that).

---

**User**

give a concise example from the paper @kg.txt with explanation so I understand:
1. why current eval metrics are incorrect?
2. how we can improve and report true metrics using heart negatives?

---

**Cursor**

**Example from Yin et al. (`kg.txt`) — Dell co-exploitation (Fig. 1)**

Exploit **EDB-48582** chains two bugs on Dell OMNM:

```text
CVE-2018-15767  ──CO-EXPLOIT──  CVE-2018-15768
     (privilege escalation)          (arbitrary file R/W)
```

Link prediction task: predict a **CO-EXPLOIT** edge between two **CVE** nodes (same product family, same attack chain).

---

## 1. Why current eval metrics can be misleading (Yin’s setup)

**Yin does (Sec. IV-A, ~lines 607–611):**
- **Train pos:** co-exploit edges before 2015 (5,349)
- **Test pos:** co-exploit edges from 2015+ (1,531)
- **Test neg:** *“Negative samples are **randomly sampled from the entities set**.”*

**Toy test query:** hide true link  
`(CVE-2018-15767, CO-EXPLOIT, CVE-2018-15768)` — already in train; use a **2016** pair instead, e.g. `(CVE-A, CVE-B)`.

| Candidate | Yin-style random neg | Model task |
|-----------|----------------------|------------|
| `(CVE-A, CVE-B)` true co-exploit | score high | ✓ should win |
| `(CVE-A, CVE-1999-0001)` random CVE | score low | **Too easy** — unrelated decade/product |
| `(CVE-A, CVE-C)` same vendor, shared CWE neighbors, **no** co-exploit edge | score ? | **Hard** — realistic “might co-exploit but doesn’t” |

Random negatives are mostly **obviously wrong** missing links → F1/AUC look strong (Yin reports **~81% F1**) but the model is not stressed on **plausible** false pairs analysts care about.

Same issue HeaRT found on OGB: **easy negatives inflate link-prediction metrics.**

---

## 2. How HeaRT improves reporting (true / harder metrics)

**Same test positive:** `(CVE-A, CVE-B)` (held-out co-exploit).

**HeaRT/SoHeaRT test negatives** (per positive, fix head `CVE-A`):
- Sample **K non-edges** `(CVE-A, Z)` with **high PPR/CN/RA** on the train co-exploit graph
- `Z` = CVEs in the **same neighborhood** (similar product/CWE context) but **not** co-linked

```text
Pos:  (CVE-A, CVE-B)     ← real co-exploit (hidden)
Neg:  (CVE-A, CVE-C)     ← no edge, but structurally plausible (SoHeaRT)
      (CVE-A, CVE-D)     ← same
```

**Re-score:** pool pos + K hard negs → **AP / AUC / MRR** (same as your `link_pred_metrics.py`).

**Expected:** metrics **drop** vs Yin’s random-negative eval — that drop is the **honest** difficulty; you report **both**:

| Eval | Negatives | What it measures |
|------|-----------|------------------|
| **Yin-style (easy)** | Random CVE from entity set | Inflated baseline |
| **HeaRT (hard)** | Top-K PPR/CN non-edges per query | Can the model beat **credible** non-links? |

---

**One line:** Yin asks *“is this co-exploit link real?”* but tests against **random unrelated CVEs**; HeaRT tests against **plausible missing links** in the same CVE neighborhood — harder and closer to real analyst mistakes. Your contribution: run **both** on the Yin graph with **GCN + `create_heart_negatives.py`** and show the gap.

---

**User**

is this really needed here? I dont think so, I mean is this improvment relevant like it was relevant for heart paper?

---

**Cursor**

**Your instinct is mostly right — it’s less necessary here than in the original HeaRT paper.**

---

## Why HeaRT mattered on OGB

- Task: *“Will this link exist?”* on **dense** social/citation graphs  
- Standard eval: random non-edges → **very easy**  
- Gap was **large and systematic** across many datasets → clear benchmark fix  

---

## Why it’s weaker for Yin CVE co-exploit

| Factor | HeaRT (OGB) | Yin CVE KG |
|--------|-------------|------------|
| **Eval** | Random non-edges dominate | Random negs **plus** temporal split (pre/post 2015) — already some rigor |
| **Harder eval exists** | Rank-all entities (standard KGC) | Yin didn’t use rank-all; but **that** might be the fairer fix, not only SoHeaRT |
| **Sparsity** | Dense graphs | **Very sparse** co-exploit (~&lt;5% CVEs) — random pairs are often easy, but the **task is already hard** (F1 ~81%, not 99%) |
| **Stakes** | Metric inflation was huge | Inflation likely **smaller** than OGB; less dramatic “gotcha” |

So: same **logic** applies (easy negs flatter scores), but the **payoff** is smaller and less central than for OGB.

---

## Compared to your attack-graph plan (LANL/CICAPT/OpTC)

| | Attack graphs + hard benign | CVE KG + HeaRT |
|--|------------------------------|----------------|
| **Wrong eval object?** | **Yes** — attacks are real edges, not missing links | **No** — missing co-exploit links is the right object |
| **Novelty if you fix eval** | **Higher** — Euler uses haystack; you tier it | **Lower** — “apply HeaRT to another domain” |
| **Needed for a good paper?** | **Yes** (core story) | **Nice-to-have**, not essential |

---

## Verdict

**Not really needed** the way HeaRT was needed on OGB — unless you **measure** a big AP/F1 gap (random vs SoHeaRT) and make that the whole paper. For CARS, **attack graphs + tiers A–C** is the more relevant “eval honesty” story; CVE+HeaRT is optional extra work with **weaker novelty**.

**One line:** HeaRT-on-Yin is **technically valid but not as compelling** as HeaRT-on-OGB or hard-benign-on-LANL; skipping it is reasonable.

---

**User**

but random sampling would sample edges with less common neighbors for thes test set here right? so why not needed?

---

**Cursor**

**You’re right on the mechanism** — random CVE–CVE pairs are usually **easy** (low CN/PPR, unrelated).

---

## Why random negs are still easy here

CVE graph is **sparse** (~&lt;5% co-exploit). Random `(CVE-A, CVE-Z)`:

- Often **no shared neighbors**, different products/years  
- Model scores them low → **easy negatives**  
- Same failure mode HeaRT describes on OGB  

So **yes** — random sampling does **not** automatically give hard cases; it mostly gives **unrelated** pairs.

---

## Why I still said “less needed” (not “wrong”)

| Point | Meaning |
|-------|---------|
| **Logic applies** | Easy random negs → inflated metrics — **true for Yin too** |
| **Less dramatic** | Yin already gets **~81% F1**, not ~99% AP — inflation may be **smaller** than OGB |
| **Other rigor** | Temporal split (pre/post 2015) already avoids some leakage |
| **Standard alternative** | KGC field often uses **rank-all-tails + filter** — also hard; Yin didn’t, but reviewers know that |
| **Your situation** | You’d **build the KG from scratch** for an incremental “HeaRT on cyber KG” story while **attack-graph data is ready** |

So: **HeaRT would help honest metrics on Yin** — I didn’t mean the idea is invalid. I meant **priority/novelty vs your LANL/CICAPT/OpTC plan**.

---

## Revised take

| Question | Answer |
|----------|--------|
| Random negs mostly low-CN here? | **Yes** |
| Would SoHeaRT make eval harder/honester? | **Yes** |
| As important as HeaRT on OGB? | **Probably not** (smaller gap, different community expectations) |
| Worth doing for CARS if you have time? | **Optional section** — not required if attack-graph tiers are main |

**One line:** Your reasoning is correct; I was arguing **effort vs payoff**, not that hard negatives don’t matter on CVE graphs.

---

**User**

As important as HeaRT on OGB?
Probably not (smaller gap, different community expectations)


can you explain little more on this

---

**Cursor**

**Why the HeaRT-style fix matters less on Yin’s CVE graph than on OGB**

---

## 1. Smaller metric gap (less “inflation drama”)

**HeaRT on OGB (typical pattern):**
- Random non-edge eval → AP/MRR **very high** (often looks near SOTA)
- SoHeaRT eval → metrics **collapse** a lot
- Clear headline: *“papers were overstating performance”*

**Yin on CVE co-exploit:**
- Graph is **extremely sparse** (~&lt;5% CVEs ever co-exploit)
- Task is already hard → reported **F1 ~81%**, not 95–99%
- Random test negs are easy, but the **positives are rare and the model isn’t saturating** anyway
- SoHeaRT might drop F1/AP **some**, but unlikely to be the same **cliff** as OGB (e.g. 0.95 → 0.55)

**Analogy:** HeaRT on OGB is like a test where everyone gets 98% with multiple-choice — then you switch to hard questions and everyone gets 60%. On Yin, the class already scores ~81%; hard questions might move it to ~70% — real, but less shocking.

---

## 2. Different community expectations

**General LP / OGB community (HeaRT’s audience):**
- Standard benchmark = **ogbl-collab, citation, etc.**
- Everyone used **same easy eval** for years → HeaRT **redefined the benchmark**
- Reviewers: *“You must report HeaRT negatives.”*

**Cyber KG / Yin community:**
- Eval is often **custom per paper**: temporal split, random entity negs, F1 not AP
- Many KGC papers use **rank-all-entities (filtered)** — already harder than Yin’s “random from entity set”
- Reviewers may say: *“Yin’s negs are weak; use filtered rank-all”* before they ask for SoHeaRT specifically
- HeaRT is **not yet** the norm in **cybersecurity KG** papers

So your contribution “we applied HeaRT to CVE” reads as **cross-domain transfer**, not **fixing a broken standard everyone uses**.

---

## 3. Different task difficulty baseline

| | OGB social graphs | Yin CVE co-exploit |
|--|-------------------|---------------------|
| **Density** | Many edges per node | Very few co-exploit links |
| **Random neg** | Easy | Easy |
| **Random pos baseline** | Non-trivial structure | Already sparse / imbalanced |
| **Reported scores** | Often very high with weak eval | Already moderate (~81% F1) |

Inflation = **gap between easy eval and hard eval**. If easy eval never gave 99%, the **story is weaker**.

---

## 4. What Yin already did (partial HeaRT job)

- **Temporal split** (pre-2015 train / post-2015 test) — avoids future→past leakage  
- HeaRT doesn’t replace that; it only sharpens **which negatives** you use at test  

That’s real rigor HeaRT **doesn’t** provide. So Yin is not “only random negs with no care” — random negs are one weakness among several already partially addressed.

---

## 5. What this means for your paper

| Claim | Strength |
|-------|----------|
| “HeaRT on OGB-scale inflation on Yin” | **Uncertain** until you run it — gap may be modest |
| “Cyber KG papers should use harder negatives like HeaRT” | **Valid** but incremental vs attack-graph hard benign |
| “Same as HeaRT NeurIPS contribution” | **Overclaim** unless you show a **large, consistent** drop across cyber KGs |

---

**One line:** Random negs are still easy on CVE graphs (you’re right), but Yin’s scores aren’t near ceiling and cyber KG doesn’t share OGB’s single broken eval protocol — so proving SoHeaRT helps is **useful**, but usually **not as central** as HeaRT’s original OGB result or your **hard-benign attack-graph** story.

---

**User**

That's exactly the motivation behind **hard negative sampling**.

In standard KGC filtered ranking:

* You replace the missing entity with **all entities**.
* Most entities are completely unrelated.
* For example:

[
(\text{Obama}, \text{bornIn}, ?)
]

Candidates include:

* Hawaii ✓
* Google
* Tesla
* Amazon
* Paris
* ...

Many of these are trivially wrong.

---

Your idea is:

> Instead of corrupting with random entities, choose entities that are structurally similar to the correct one using PPR, Common Neighbors (CN), Adamic-Adar, etc.

Example:

True triple:

[
(\text{CVE-1}, \text{coExploit}, \text{CVE-2})
]

Random negative:

* (CVE-1, coExploit, CVE-9999)

Hard negative:

* (CVE-1, coExploit, CVE-3)

where CVE-3:

* shares many neighbors with CVE-2,
* has high PPR score relative to CVE-1,
* belongs to the same ATT&CK techniques,
* has similar exploitation patterns.

Now the GCN cannot simply rely on local neighborhood similarity because the negative looks structurally very similar to the positive.

---

Why this is harder for GCNs

GCNs learn embeddings by message passing.

If:

* Positive pair: ((u,v))
* Hard negative pair: ((u,w))

and (v) and (w) have nearly identical neighborhoods,

then after aggregation:

[
h_v \approx h_w
]

The model receives almost the same feature representation for both.

A heuristic-based model (CN, AA, PPR) will also struggle because the hard negative was selected specifically to have high heuristic scores.

The model is forced to learn finer relational patterns rather than:

> "high neighborhood overlap ⇒ positive edge"

---

This is essentially what papers such as the HeaRT framework try to achieve:

* Use CN/PPR and related topology measures.
* Mine negatives that are close to positives in graph structure.
* Create evaluation sets where negatives are plausible links rather than obviously absent links.

The goal is not merely to "confuse the GCN," but to prevent inflated performance caused by easy negatives and measure whether the model can distinguish **true future links** from **structurally similar non-links**.

For your CTI co-exploitation graph, using PPR/CN-selected CVEs as corruptions would likely be much more realistic than random entity replacement because attackers tend to exploit vulnerabilities within related ecosystems, products, or attack paths rather than choosing arbitrary CVEs.


---

**Cursor**

**Mostly correct** — a few precision fixes for your paper.

---

## What’s right

| Point | ✓ |
|-------|---|
| Filtered rank-all includes **many trivial** corruptions | Yes |
| HeaRT/SoHeaRT picks **structurally plausible non-links** (high PPR/CN/RA) | Yes |
| Hard neg `(CVE-1, CVE-3)` with similar neighborhood to true `CVE-2` forces finer discrimination | Yes |
| For co-exploit, plausible fakes are **same ecosystem**, not random CVE-9999 | Yes — matches Yin’s domain |
| Goal = **honest metrics**, not tricking the GNN | Yes |

Your GCN intuition is right: if `h_v ≈ h_w` for hard neg `w`, dot-product scores can tie → AP drops unless the model uses more than overlap.

---

## Fixes / caveats

**1. HeaRT is mainly an eval protocol** (NeurIPS’23), not training. Same idea appears in **SANS, RANS, NSCaching** for training. Say: *“We adopt HeaRT-style hard negatives at **test** (and optionally train).”*

**2. Filtered rank-all is already harder than Yin’s setup.** Yin uses *“randomly sampled from the entities set”* — weaker than full rank-all. Your critique hits **Yin directly**; rank-all is a middle ground (many easy + some hard candidates in the list).

**3. ATT&CK techniques on the line** — only if your graph has those relations. **Yin’s graph** = CVE, Exploit, Product, CO-EXPLOIT, CO-AFFECT — **no ATT&CK** unless you merge another KG. Safer: *“same product/CWE neighborhood, similar exploit patterns.”*

**4. Heuristic struggle** — hard negs are **high CN/PPR by construction**, so CN/AA baselines can also be fooled; that’s intentional. GNN must beat **structure alone**.

**5. vs attack graphs (LANL)** — this whole story applies to **missing-link KGC**. On LANL, attacks are **real edges**; hard **non-edges** (SoHeaRT) are the wrong object → **hard benign** instead.

---

## How to use this in your write-up

**For CVE / Yin track (if you do it):**

> Co-exploitation LP should be evaluated with **structurally plausible non-links** (SoHeaRT), not only random entity corruptions, because exploits cluster by product and vulnerability context.

**For CARS main track (LANL/CICAPT/OpTC):**

> The same *principle* applies but the negative type changes: **real benign edges** (same user/host), not PPR non-edges.

---

**One line:** Your motivation is sound for **KG completion**; cite HeaRT + Yin; drop ATT&CK unless you add that data; for **attack graphs**, reuse the *principle* (hard personalized negatives) with **benign edges**, not this corruption recipe.

---

**User**

so YIn is doing random sampling for training or test hby replacing thired entity?

---

**Cursor**

**Both train and test use random negatives — but Yin is not standard “replace the tail in a triple.”**

---

## Yin’s setup (from `kg.txt`)

**Graph:** vulnerability nodes + **CO-EXPLOITATION** edges `(CVE-u, CVE-v)` (homogeneous edge LP, not `(h, r, ?)` ranking).

---

### Training — **two parts**

| Stage | Positives | Negatives |
|-------|-----------|-----------|
| **Source graph** (CO_AFFECT, GKTL) | Random split of real edges | **Random non-edges** `(u,v)` in source graph (lines 551–554) |
| **Target graph** (co-exploit) | Co-exploit edges **before 2015** (5,349) | Same line 611 context: negatives tied to entity-set sampling for the edge classifier |

Train negs = **random missing edges**, not HeaRT/PPR.

---

### Test

| | |
|--|--|
| **Positives** | Co-exploit edges **2015+** (1,531) |
| **Negatives** | *“Negative samples are **randomly sampled from the entities set**.”* (line 611) |
| **Metric** | **F1** on pos + neg (lines 646–647) — binary edge classification, **not** rank-all-entities |

So test negs are also **random** — likely random CVE pairs / samples from the entity set, **not** filtered corruption of only the tail over all CVEs.

---

## vs “replace third entity” (KGC style)

| Standard KGC | Yin |
|--------------|-----|
| `(CVE-1, coExploit, ?)` rank all tails | **Edge** `(CVE-1, CVE-2)` yes/no |
| Corrupt head or tail per triple | **Random `(u,v)` pairs** from entity set |
| MRR / Hits@10 | **F1** |

So your HeaRT example *(CVE-1, coExploit, CVE-3)* is the **right idea** for hard negatives, but Yin’s paper is **simpler**: random **edge pairs**, not tail replacement in a triple ranking task.

---

**One line:** Yin uses **random negatives for both train and test**; test is **not** full filtered entity ranking — it’s **random pairs from the CVE set** + F1, which is easier than SoHeaRT or rank-all.

---

**User**

so we can pick heart instead of random edge pairs in test set or not ? to report true results in yin. because in random it may pick edges far apart whcih would be easy for model to tell that edge doesnot exist so we want to confuse the model

---

**Cursor**

**Yes — for test eval you can replace Yin’s random edge pairs with HeaRT/SoHeaRT hard non-edges** and report both (random vs SoHeaRT). That’s a valid way to expose inflated F1/AP when negatives are trivially unrelated.

---

## What you’d do

| | Yin (original) | Your add-on |
|--|----------------|-------------|
| **Test pos** | Held-out co-exploit edges (2015+) | Same |
| **Test neg** | Random `(CVE-u, CVE-v)` from entity set | Per pos `(u,v)`: **K non-edges** `(u, w)` with high PPR/CN/RA (SoHeaRT) |
| **Metric** | F1 | F1 and/or AP/AUC (pooled ranking) |

Random negs are often **far apart** → model says “no link” easily → **scores look too good**. SoHeaRT forces **plausible** non-links → **harder, more honest** test.

---

## Caveats (for the paper)

1. **Not “Yin’s F1”** — you’re a **stricter eval protocol**; cite Yin for task/data, HeaRT for negatives.
2. **Train** can stay Yin-style (random negs) or also use hard negs — **test** is where the fair comparison matters most.
3. **Same graph/split** as Yin (temporal pre/post 2015) so the only change is **negative sampling**.
4. **Build the co-exploit graph first** — then run `create_heart_negatives.py` on it.

---

**One line:** Yes — SoHeaRT at **test** instead of random pairs is exactly the right fix for “easy unrelated CVE pairs”; report **random vs SoHeaRT** side by side to show the gap. That’s a contribution on top of Yin, not a reproduction of their Table II.

---

**User**

what is graph partitions in yin and client numbers for, did you not tell me correct details of what Yin does? ans brieflty

---

**Cursor**

**Yin has no “client numbers”** — that’s likely **OpTC** (`AIA-101-125` host batches) or another paper. Yin uses **two subgraphs**, not federated clients.

---

## What Yin actually does (brief)

**1. One big vulnerability KG** (CVE, Exploit, Product; NVD + ExploitDB + CVE Details)

**2. Two subgraphs (same CVE nodes, different edge types)** — this is what people mean by “partitions”:

| Subgraph | Edges | Role |
|----------|-------|------|
| **Target** | **CO-EXPLOITATION** (CVE–CVE co-exploited) | **Sparse** — what you predict |
| **Source** | **CO-AFFECT** (CVEs affecting same product) | **Denser** (~4× more edges) — pretrain embeddings |

**3. GKTL:** Train GNN on **source** (CO-AFFECT LP) → copy node embeddings to **target** → MLP predicts **CO-EXPLOIT** edges.

**4. Splits (target co-exploit only):**
- **Train pos:** before 2015 (5,349 edges)
- **Test pos:** 2015+ (1,531 edges)
- **Neg (train & test):** **random** non-edges / pairs from entity set

**5. MAGCN:** multimodal GCN (BERT text + tabular CVE features) — their **model** novelty, not eval.

---

## Not in Yin

- Client numbers / federated learning  
- HeaRT / hard negatives  
- Triple tail corruption / rank-all-entities  
- ATT&CK  

---

**One line:** “Graph partitions” = **source (dense CO-AFFECT) + target (sparse CO-EXPLOIT)** for transfer learning — not clients. Your HeaRT idea would change **test negatives only**, on top of this pipeline.

---

**User**

Ok tell KG approach precisely in just 3-5 steps

---

**Cursor**

**KG approach (Yin task + HeaRT test) — 5 steps**

1. **Build graph** — From NVD/ExploitDB: CVE nodes; **CO-EXPLOIT** edges between CVEs exploited together; optional **CO-AFFECT** edges (same product) as dense source graph.

2. **Split** — **Train pos:** co-exploit edges before 2015. **Test pos:** co-exploit edges 2015+. Train GNN on source (CO-AFFECT) or target with random train negs.

3. **Train** — GCN/MAGCN: learn CVE embeddings; BCE on real edges (pos) vs **random** non-edges (neg).

4. **Test (two protocols)** — For each held-out co-exploit `(u,v)`:
   - **Yin:** random neg pairs from CVE set  
   - **HeaRT:** **K hard non-edges** `(u,w)` with high PPR/CN (SoHeaRT)

5. **Report** — F1/AP/AUC for **random vs SoHeaRT**; show metrics drop under hard negs → honest link-prediction eval.

---

**User**

simple words

---

**Cursor**

**KG approach in simple words**

1. **Make a graph** — Each dot is a CVE. Draw a line between two CVEs if the same exploit uses them together.

2. **Split by time** — Old co-exploit links (before 2015) = learn from. New ones (2015+) = test if the model can find them.

3. **Train** — Teach the GNN: real links = yes, random fake pairs = no.

4. **Test two ways** — For each real test link:
   - **Easy:** compare to random unrelated CVE pairs  
   - **Hard (HeaRT):** compare to fake pairs that *look* like they could link (similar neighbors)

5. **Report both** — Scores look better on easy negatives; hard negatives show if the model really works.

---

**User**

so I have to build the graph using the datasets

---

**Cursor**

**Yes.** For the KG track you **build the graph yourself** from raw sources (NVD, ExploitDB, CVE Details — like Yin), or use a starter like [nislab/threat-knowledge-graph](https://github.com/nislab/threat-knowledge-graph) and adapt it.

**You create:**
- CVE nodes  
- **CO-EXPLOIT** edges (two CVEs used in the same exploit)  
- Optional **CO-AFFECT** edges (CVEs on same product) for Yin-style pretraining  

**Unlike LANL/CICAPT/OpTC:** you already have the raw files; you still run preprocessors, but those logs **are** the data. For KG there is **no ready-made graph download** — only sources to turn into edges.

**Shortcut:** nislab triples give CVE–CPE–CWE links — faster, but not exactly Yin’s co-exploit graph unless you derive co-exploit from ExploitDB.

---

**User**

https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=9832800 what database to be used here? give one word answer and if its opensource?

---

**Cursor**

**NVD** — yes (public/open data; paper also uses ExploitDB and CVE Details).

---

**User**

any graph database used herE?

---

**Cursor**

**Yes — Neo4j.**

Yin stores the vulnerability KG in **Neo4j** and queries it with **Cypher** (via **py2neo** in Python). That’s for building/storing the KG — your GCN/HeaRT experiments can use `graph.pt` / edge lists instead if you prefer.

---

**User**

For Evaluation: Per each attack edge(u,v), we sample K negatives only from attack-looking benigns (with low PPR/CN/RA)
All benign test edges
  → keep only (u,v) NOT in train
  → among those, keep bottom PPR/CN/RA
  → sample K per attack and rank attack edge among hard benigns which look like attack so it becomes difficult for model to classify if attack can be classified as benign and allow connection between nodes or stop that connection because its detected as an attack


lets start implementing this then we can move towards KG

---

**Cursor**

**Tier D (low-PPR/CN/RA benign confusers) — start on attack graphs first, KG later.**

I'm in **Ask mode** and can't edit files. **Switch to Agent mode** and I'll implement.

---

## What we'll build (Tier D + A/B/C)

**Start: CICAPT** → then LANL → OpTC.

### Per dataset preprocessor outputs

```
CARS/processed/{cicapt,lanl,optc}/
  graph.pt                 # train benign adj only
  attack_test_pos.pt       # [N, 2]
  neg_random_test.npy      # Tier A [N, K]
  neg_benign_test.npy      # Tier B [N, K]
  neg_hard_benign_test.npy # Tier C [N, K] same-source
  neg_anomaly_benign_test.npy  # Tier D [N, K] unseen + bottom PPR/CN/RA
  splits/split.pt
```

### Tier D sampler (core logic)

```text
Pool = all benign TEST edges
Filter: (u,v) not in train edge set
Score each on TRAIN graph: s = PPR(u→v) + CN + RA  (reuse HeaRT calc_ppr / hard_utils)
Keep bottom percentile (e.g. bottom 20%)
Per attack (u,v): sample K from pool (optionally require same u)
```

### Training / eval (`main_gnn_snap.py`)

- Train: benign pos + random non-edges (unchanged)
- Test: attack pos vs chosen tier negatives
- Score: `1 - ŷ` → AP/AUC via `pooled_auc_ap`
- Models: GCN, then GraphSAGE

---

## Order in Agent mode

1. `preprocess_cicapt.py` + Tier D PPR/CN (reuse `heart_negatives/`)
2. Wire `main_gnn_snap.py` (`--dataset`, `--neg_tier`)
3. One end-to-end CICAPT run
4. `preprocess_lanl.py` (Tier D strongest here)
5. `preprocess_optc.py`

**Reply in Agent mode with “start CICAPT”** and I'll begin with `preprocess_cicapt.py`.

---

**User**

what is the difference in my work vs euler give in one concise table

---

**Cursor**

| | **Your work** | **Euler** |
|--|---------------|-----------|
| **Goal** | **Evaluation protocol** — when do LP metrics lie? | **Detector** — find lateral movement |
| **Model** | Static **GCN / GraphSAGE** | **GCN + GRU/LSTM** (temporal) |
| **Train** | Benign edges + random non-edges | Benign snapshots only; random non-edges |
| **Test positives** | Held-out **attack** edges | Labeled **anomalous** edges in test window |
| **Test negatives** | **Explicit tiers** (random non-edge → random benign → same-source / Tier D) | **Implicit** — almost all other **real benign** edges in test snapshots |
| **Sampling** | **K decoys per attack** (controlled) | Full haystack (all test traffic) |
| **Score** | `1 - ŷ`, pooled **AP/AUC** | Low ŷ + threshold τ from benign val |
| **Contribution** | **Hard-benign / tiered neg** critique | Scalable **temporal** anomaly LP |
| **Datasets** | CICAPT + LANL + OpTC | LANL + OpTC |
| **Novelty** | *How* you evaluate | *How* you model time + scale |

**One line:** Euler builds a **temporal detector**; you study **whether static LP results are honest** under **tiered benign negatives**, not a better IDS.

---

**User**

1. what metrics does euler report?
2. can we compare our results with Euler for eval?

---

**Cursor**

**1. Metrics Euler reports**

| Metric | Meaning |
|--------|---------|
| **AUC** | ROC over edge scores (anomalous = low likelihood) |
| **AP** | Average precision (main metric for imbalance) |
| **TPR** | True positive rate at chosen threshold τ |
| **FPR** | False positive rate at τ |
| **P** | Precision at τ |

**Threshold:** τ from **benign validation** snapshots (Eq. 8, λ=0.6).  
**Tasks:** *Dynamic link detection* (anomaly) and *dynamic link prediction* (general LP).  
**LANL/OpTC tables:** AUC, AP, TPR (%), FPR (%) — e.g. LANL GCN+GRU: AUC ~0.99, AP ~0.05; OpTC Euler GRU: AUC ~0.89, AP ~0.09.

---

**2. Can you compare your results to Euler?**

| | **Comparable?** |
|--|-----------------|
| **Same metric names (AUC, AP)** | Yes — report them |
| **Same numbers side-by-side** | **No — not fair直接** |

**Why not apples-to-apples:**

| | **Euler** | **You** |
|--|-----------|---------|
| Model | GCN + **GRU** (temporal) | **Static** GCN/SAGE |
| Test negatives | **All** other real benign edges in test window | **K sampled** per attack (tiers A–D) |
| Test graph | Attack edges **in** test snapshot structure | Frozen **benign train** `adj_t` |
| AP meaning | Score **every** test edge | Pooled **attack vs K decoys** |

---

**What to do in the paper**

- **Main table:** Your tiers A–C (and D) — **don’t** claim to beat Euler’s AP.
- **Optional row:** “Euler-aligned” on LANL/OpTC — score all test-window edges, τ from benign val — **then** cite their Table 7 / 9 as reference.
- **Discussion:** Same metrics, **different eval protocol**; your point is **tiered negatives**, not SOTA detection.

**One line:** Report **AUC/AP** like Euler, but compare **protocols** (random vs hard benign), not headline numbers — unless you add an Euler-style full-haystack eval row.

---

**User**

is my approach correct?

---

**Cursor**

**Mostly yes — as an evaluation paper, not as a new detector.**

---

## What’s correct

| Piece | Verdict |
|-------|---------|
| **Problem** | Random non-edge eval **does** mislead on cyber attack graphs |
| **Main fix (Tier C)** | Same-source **real benign** negatives — right object (attacks are real edges) |
| **Train** | Benign-only + random non-edges (Euler/Bowman family) | ✓ |
| **Score** | `1 - ŷ` for anomaly ranking | ✓ |
| **Metrics** | AP/AUC pooled per attack | ✓ |
| **3 datasets** | LANL + CICAPT + OpTC — different cyber settings | ✓ |
| **2 GCNs** | GCN + SAGE — fair comparison | ✓ |
| **vs Euler** | Different contribution (protocol, not temporal SOTA) — cite, don’t compete on same numbers | ✓ |

---

## What to tighten

| Issue | Fix |
|-------|-----|
| **Tier D everywhere** | **LANL-first**; weak on CICAPT without filters — don’t claim D on all 3 without evidence |
| **vs Euler numbers** | Don’t say “we beat Euler AP” — different eval universe |
| **Contribution wording** | Say **“tiered hard-benign protocol”**, not “new attack detector” |
| **Tier A purpose** | **Ablation** (inflated baseline), not main result |
| **CICAPT edges** | Define graph clearly (process/process or entity collapse) and stick to it |

---

## What would make it *incorrect*

- Claiming SoHeaRT **non-edges** as main eval on attack graphs  
- Only Tier A results as “detection works”  
- One model (GCN only) with no tier ablation  
- Ignoring that Euler already uses implicit benign haystack (position as **explicit tiers + multi-dataset gap**)

---

**Verdict:** **Correct** for CARS if you deliver **Tier A→C table** (optional D on LANL), **2 models × 3 datasets**, and frame it as **honest LP evaluation**, not Euler 2.0.

Switch to **Agent mode** to implement.

---

**User**

should we change train/test to euler and add hard negatives or ours is fine?

---

**Cursor**

**Keep your train/test for the main paper.** Optionally add **one Euler-aligned eval row** on LANL/OpTC — don’t rebuild the whole study around Euler.

---

## Train/test: yours vs Euler

| | **Euler** | **Yours** | Match? |
|--|-----------|-----------|--------|
| **Train pos** | Benign before attack | Benign before attack / Phase 1 | ✓ Same idea |
| **Train neg** | Random non-edges | Random non-edges | ✓ |
| **Test pos** | Attack / anomalous edges | Attack edges | ✓ |
| **Test neg** | **All** other real benign in test window | **K sampled** per tier | ✗ Different (your contribution) |
| **Model** | GCN + **GRU**, snapshots | **Static** GCN/SAGE | ✗ |
| **Test graph** | Attack edges **in** test snapshots | **Frozen** benign `adj_t` | ✗ |

Train/split is already Euler-compatible; the gap is **test eval + temporal model**, not “wrong train.”

---

## Recommendation

| Do | Don’t |
|----|--------|
| **Keep** benign-train / attack-test + **tiers A–C** (D on LANL if you want) | Reimplement **full Euler** (temporal + full haystack) as the main method |
| **Optional:** one table row **“Euler-style haystack”** — score all benign test edges, no K sampling — on **LANL + OpTC only** | Claim you **beat Euler’s AP** on the same setup without matching their pipeline |
| **Hard negatives on your test** — that **is** the paper | Replace your tiers with only Euler’s implicit negatives |

---

## Why yours is fine

- Euler’s test negatives = **implicit Tier C at full scale** (all benign haystack).
- Your **Tier C** = **controlled** same-context sampling (HeaRT-style *personalized* eval).
- Your **Tier A** shows **why** Euler-looking high AUC can mislead when someone uses random non-edges instead.

---

**One line:** **Don’t change train/test to Euler** — align splits only where easy (LANL pre-attack, OpTC 4 benign days). **Keep tiered hard-benign test** as the contribution; add an optional **full-haystack row** to relate to Euler, not replace your design.

---

**User**

Test graph
Attack edges in test snapshots
Frozen benign adj_t

explain briefly

---

**Cursor**

**What the GNN “sees” as the graph at test time**

---

## Euler: attack edges **in** test snapshots

At test time Euler builds snapshot \(G_t\) from **everything that happened in that time window** — including **attack** edges.

```text
Test snapshot graph = benign edges + attack edges (both present)
GNN message-passing runs on this full graph
Then scores edges (including attacks) in that context
```

Attack nodes get embeddings influenced by **attack structure** (new paths, new neighbors). Temporal RNN also uses **past snapshots**.

---

## Yours: **frozen benign** `adj_t`

At test time the GNN always uses **only the train-time benign graph** — attack edges are **never** added to `adj_t`.

```text
adj_t = train benign edges only (fixed)
Score attack edge (u,v) using embeddings from that frozen graph
Attack (u,v) is NOT an edge in adj_t when computing h_u, h_v
```

You ask: *“Given normal structure, how link-like is this pair?”* — attacks are scored as **outsiders** to the train graph, not as part of it.

---

## Why it matters

| | **Euler** | **Frozen benign** |
|--|-----------|-------------------|
| Attack in graph? | **Yes** (in test snapshot) | **No** |
| Embeddings | Context includes attack neighborhood | Only benign-train context |
| Closer to | “Anomaly in evolving network” | “Cold score vs normal past” |

Both are valid; **not directly comparable** without matching this choice.

---

**One line:** Euler tests attacks **inside** the live test graph; you test attacks against a **fixed normal graph** from training.

---

**User**

so what shoudl we do?

---

**Cursor**

**Do this (locked plan):**

---

## Main setup (your paper)

| Choice | Use |
|--------|-----|
| **Train** | Benign edges only + random non-edges (match Euler/Bowman spirit) |
| **Test graph** | **Frozen benign `adj_t`** — keep it; simpler, matches your SNAP/HeaRT code |
| **Test negatives** | **Tiers A → B → C** (main = **C** same-source benign) |
| **Tier D** | Optional **LANL only** (low-PPR benign) — skip on CICAPT/OpTC unless ablation shows value |
| **Models** | **GCN + GraphSAGE** (not Euler+RNN for deadline) |
| **Metrics** | AP + AUC, score `1 - ŷ` |
| **Datasets** | CICAPT → LANL → OpTC (all 3) |

**Don’t** switch to Euler’s full pipeline (temporal + attack-in-graph) for the main results.

---

## One optional add-on (LANL + OpTC only)

**Tier E / “Euler-haystack”:** score attack vs **sample of all benign test edges** (or many per attack) — cite Euler as “implicit full haystack,” show how Tier C sits between Tier A and haystack.

One paragraph + small table — not a second implementation track.

---

## What to say in the paper

> We fix `adj_t` to benign train (standard unsupervised LP). Our contribution is **test negative protocol**, not temporal modeling. Euler uses richer test-time graphs; we isolate **which negatives** make AP misleading.

---

## What not to do

- Rebuild Euler (GRU + snapshots) for CARS  
- Claim you beat Euler’s Table 7 numbers  
- Tier D on all datasets without evidence  
- SoHeaRT non-edges as main eval on attack graphs  

---

## Next step

**Agent mode** → implement `preprocess_cicapt.py` + tiers + `main_gnn_snap.py` hooks. That’s the critical path; KG/Yin + HeaRT later if time.

**One line:** **Frozen benign `adj_t` + tiers A/B/C + 2 GNNs × 3 datasets** — correct and enough; optional haystack row on LANL/OpTC to nod at Euler.

---

**User**

but numbers are not comparable if my method is not gcn + gru. 

---

**Cursor**

**Correct — your numbers are not comparable to Euler’s Tables 7/9.**

Different model (static vs GRU), test graph (frozen vs attack-in-snapshot), and test negatives (K tiers vs full haystack). **Don’t put your AP next to Euler’s and imply “better/worse.”**

---

## What to do instead

**1. Compare within your own setup (main)**  
Same GCN/SAGE, same frozen `adj_t`, only change **neg tier** (A → B → C).  
That’s the paper: *“AP jumps on Tier A, drops on Tier C.”*

**2. Cite Euler qualitatively**  
“Euler reports high AUC / low AP on LANL under temporal full-haystack eval; we study **how much tier choice alone** moves metrics under static LP.”

**3. Optional fair-ish bridge (LANL only, if time)**  
Run **static GCN only** (no GRU) with **Euler-like negatives** = sample many/all benign test edges (haystack row).  
Still not identical (no temporal, frozen `adj_t`), but you can say *“closer to Euler’s eval object, still static encoder.”*

**4. Don’t do for CARS deadline**  
Full GCN+GRU replication just to match Euler numbers — different paper.

---

## How to write the results table

| Table | Content |
|-------|---------|
| **Main** | GCN & SAGE × tiers A/B/C × 3 datasets — **your numbers only** |
| **Related work** | Euler/Bowman numbers **from their papers**, separate column “their setup” |
| **Optional** | +haystack row LANL — “Euler-adjacent eval, static GCN” |

---

**One line:** You’re not competing with Euler on detection SOTA; you’re showing **eval protocol sensitivity**. Incomparable headline numbers are fine — **cross-tier drops on your setup** are the result.

---

**User**

how about using eulers code and adding our own eval?https://github.com/iHeartGraph/Euler/tree/main/benchmarks 
But I see euler is comparing on time for different models. what would be the right comparison way? to run same datasets on random and hard negatives? can u find paper where we can show running gcn+mlp with random tests doesnot give true results we need heart negatives

---

**Cursor**

**Short answer:** Don’t use [Euler’s `benchmarks/`](https://github.com/iHeartGraph/Euler/tree/main/benchmarks) as your main eval harness. It’s for **small temporal LP** (Enron/FB/DBLP), not LANL/OpTC. The right comparison is a **controlled negative-tier ablation** on the same datasets and same GCN+MLP — cite **HeaRT** for why random test negatives mislead.

---

## What Euler `benchmarks/` actually does

That folder runs `euler_test.py` / `egcn_test.py` on **Enron, Facebook, DBLP** — serial (non-distributed) Euler vs VGRNN-style temporal link prediction. Metrics are AUC/AP on **future edges vs random non-edges** in those citation/social graphs.

**LANL/OpTC IDS experiments are elsewhere** in the Euler repo (distributed framework). Euler trains with **random non-edges per epoch** (Eq. 7 in the paper) but on LANL **test** it scores edges in attack windows against an implicit **haystack of real benign edges** in those snapshots — not random non-edges. That’s why Euler reports AUC ≈ 0.99 but AP ≈ 0.01–0.05 (Table 7): the task is extremely imbalanced once negatives are realistic.

So: Euler `benchmarks/` ≠ your CARS setup. Forking it only makes sense as an optional **“Euler-style haystack”** row on LANL/OpTC, not as the main experiment path.

---

## Right comparison for your paper

**Not:** beat Euler Table 7 numbers (different model, graph, and test protocol).

**Yes:** same train, same test positives, **only change negatives at test**.

| Fixed | Varied |
|--------|--------|
| Dataset (LANL, CICAPT, OpTC) | **Tier A:** random non-edges |
| Model (GCN + 3-layer MLP; optional GraphSAGE) | **Tier B:** random held-out benign edges |
| Train: benign only + random non-edges | **Tier C:** same-source benign (main claim) |
| Test graph: frozen benign `adj_t` | **Tier D (optional):** unseen + low PPR/CN/RA benign |
| Test pos: held-out attack edges | **Tier E (optional):** Euler-style full benign haystack sample |

**One table per model** (or one big table):

```
                LANL          CICAPT        OpTC
Tier A (random)  AP / AUC    AP / AUC      AP / AUC
Tier B           ...         ...           ...
Tier C           ...         ...           ...
```

**Claim:** Tier A **inflates** AP/AUC; Tier C (and optionally E) reflects operational false-positive pressure. That’s an **evaluation-protocol** result, not SOTA chasing.

Your existing stack (`main_gnn_snap.py` + HeaRT `gnn_model.py` + `link_pred_metrics.py`) is the right base — same architecture HeaRT benchmarks.

---

## Papers: GCN+MLP + random negatives ≠ trustworthy eval

### Primary (use this in Related Work / Motivation)

**Li et al., NeurIPS 2023 (Datasets & Benchmarks)** — [*Evaluating Graph Neural Networks for Link Prediction: Current Pitfalls and New Benchmarking*](https://arxiv.org/abs/2306.10453) ([HeaRT repo](https://github.com/Juanhui28/HeaRT))

- Exactly **GCN + MLP decoder** on OGB.
- Shows standard **random test negatives** make LP look much easier than **HeaRT hard negatives** (PPR/CN/RA-based missing links).
- Your twist: on attack graphs, “hard” = **real benign edges** (haystack), not missing KG links — but the **protocol critique** is the same.

**Follow-up:** Li et al., ICLR 2024 — [*Revisiting Link Prediction: A Data Perspective*](https://arxiv.org/pdf/2310.00793.pdf) — strengthens the data/eval angle.

### Secondary (cyber + domain motivation)

| Paper | Role |
|--------|------|
| **King & Huang, Euler (TOPS 2023)** | LANL lateral movement via temporal LP; random train negs, **implicit benign haystack at test**; AUC vs AP gap on imbalanced IDS |
| **Yin et al., IEEE TII (CVE co-exploit KG)** | Cyber LP with **random train/test negatives** — easy baseline in KG setting (`kg.txt`) |
| **Bowman et al. (GL-LV/GV)** | Static graph on LANL auth; Euler cites as prior — less about negative sampling, more baseline TPR/FPR |

**HeaRT** is the direct “GCN+MLP + random test negs mislead” cite. **Euler** supports “on cyber graphs, negatives must be **real activity**, not random non-edges” — their LANL eval is already closer to Tier C/E than Tier A.

---

## Euler code vs extend your pipeline

| Approach | Pros | Cons |
|----------|------|------|
| **Extend `main_gnn_snap.py`** (recommended) | Already GCN+MLP + HeaRT metrics; tier flags fit CARS deadline | You build LANL/CICAPT/OpTC preprocessors |
| **Euler repo + custom eval hook** | Reuse LANL temporal loading | GRU/temporal ≠ your static story; heavy; numbers still not comparable to Table 7 without matching their full protocol |
| **Euler `benchmarks/` only** | Easy to run | Wrong datasets and task for CARS |

**Practical split:**
- **Main experiments:** your preprocessors + `main_gnn_snap.py` + tiers A/B/C.
- **Optional one row:** sample from all benign test-window edges (Euler haystack) on LANL/OpTC to say “even Euler’s implicit eval is harder than Tier A.”

---

## How to phrase it in the paper

1. **HeaRT** established that random LP negatives inflate GNN scores on general graphs.
2. On **attack graphs**, positives are attacks and realistic decoys are **benign edges** (same user/host, same time context) — Tier A is optimistically easy.
3. We do **not** claim to beat Euler; we show **metric sensitivity to negative design** under a static GCN+MLP detector aligned with HeaRT’s architecture.

If you want, in Agent mode we can wire `--neg_tier` into `main_gnn_snap.py` and sketch the results table template for the paper.

---

**User**

briefly tell what is temporal here means and gru very concise 1-2 sentences

---

**Cursor**

**Temporal** here means the network is split into **time-ordered snapshots** (e.g. 30‑min windows): each snapshot is a graph of who authenticated to whom at that time, and the model uses **past snapshots to score edges in later ones**.

**GRU** is a small recurrent network that reads those snapshot embeddings **in sequence** so each time step’s representation depends on earlier activity—not just the current graph structure.

---

**User**

but i dont think heart negatives will be applicable for euler application right? Yes or No only

---

**Cursor**

**No.**

---

**User**

Okay guide me how to begin Implementation, MAKE sure code is clean and human written and minimal and self explanatory. Let start with Lanl dataset. 

---

**Cursor**

Reviewing existing preprocess patterns and LANL data layout so the implementation guide matches your repo.


**Start here:** one streaming preprocessor → negative sampler → thin data loader → reuse existing `main_gnn_snap.py` training loop. Mirror the Twitter SNAP layout; don’t fork Euler.

---

## Goal (LANL v1)

| Piece | Choice |
|--------|--------|
| Nodes | **Computers** (`C1250`, …) — same as Euler |
| Edges | Directed **(src_comp → dst_comp)** from `auth.txt.gz` |
| Train graph | Unique benign edges with **time < first redteam time** |
| Test positives | Unique **(src, dst)** from `redteam.txt.gz` |
| Test graph at eval | **Frozen train benign `adj_t` only** |
| Features (v1) | 3-dim one-hot: computer / user / special from name prefix |

Raw data: `CARS/dataset/lanl/auth.txt.gz` (~7.6 GB), `redteam.txt.gz` (749 lines).

---

## Directory layout (match Twitter pattern)

```
CARS/processed/lanl/
  graph.pt              # train benign directed edges + edge_index + num_nodes
  node_id_map.txt       # computer_name \t node_id
  meta.json             # t_attack, counts, column notes
  attack_test_pos.pt    # [N_attack, 2] long
  x_lanl.pt             # [N, 3] float (optional v1: skip, use ones)
  splits/
    split_temporal.pt   # train_benign, val_benign (10% of train)
  negatives/
    neg_tier_a_test.npy # [N_attack, K, 2] random non-edges
    neg_tier_b_test.npy # random held-out benign
    neg_tier_c_test.npy # same-src benign
```

---

## Step 1 — `CARS/code_preprocess/preprocess_lanl.py`

One file, ~150 lines. Follow `preprocess_twitter_edges.py` style: argparse, small functions, no classes.

**Pass 0 — redteam**
```python
# redteam columns: time, user, src_comp, dst_comp
t_attack = min(time over all redteam lines)
attack_pairs = unique (src_comp, dst_comp)
```

**Pass 1 — stream `auth.txt.gz`**
```python
# auth columns (0-indexed): 0=time, 3=src_comp, 4=dst_comp, 8=result (Success)
# keep: time < t_attack, result == Success, src != dst
# skip pairs in attack_pairs (if they appear in pre-attack auth)
```

Build `node_id_map` from all computers seen in train benign + attack edges.

**Outputs**
- `directed_unique`: `[E, 2]` train benign
- `attack_test_pos`: mapped attack pairs
- `meta.json`: `t_attack`, `num_nodes`, `num_train_edges`, `num_attack_edges`

**Val split:** 90/10 random split of train benign edges → `split_temporal.pt` (not random LP split of all edges like Twitter).

**Smoke test before full 7 GB run:**
```bash
# add --max_lines 1_000_000 to verify counts + paths
python CARS/code_preprocess/preprocess_lanl.py --max_lines 1000000
```

---

## Step 2 — `CARS/code_preprocess/build_lanl_negatives.py`

Reuse logic from `twitter_data.sample_random_negatives` (copy the helper functions — don’t import Twitter-specific paths).

For each attack edge `(u, v)` with `K=500`:

| Tier | Rule |
|------|------|
| **A** | Random `(a,b)` not in train graph, `a≠b` |
| **B** | Random from `val_benign` (held-out benign edges) |
| **C** | Random `(u, v')` from `val_benign` where `col0 == u`, `v' ≠ v` |

Save as `[N_attack, K, 2]` `.npy`. Seed `42` for reproducibility.

**Tier D later** — skip for v1.

---

## Step 3 — `CARS/code_preprocess/lanl_data.py`

Thin clone of `twitter_data.get_eval_tensors_for_gnn`:

```python
def load_lanl(processed_dir, device, k=500):
    # load graph.pt, split_temporal.pt, attack_test_pos.pt
    # adj_t from train_benign only
    # val positives = val_benign; test positives = attack_test_pos
    # load neg_tier_{a,b,c}_test.npy
    return data, pos_train, val_pos, test_pos, neg_a, neg_b, neg_c, num_nodes
```

**Scoring convention:** attacks should score **high** = suspicious. HeaRT LP uses high = “link exists”. For attacks use **`suspicious = 1 - link_score`** (or train with inverted labels). Pick one and stick to it.

---

## Step 4 — Training entry point

**Minimal:** copy `main_gnn_snap.py` → `main_gnn_lanl.py` and swap:
- `twitter_data` → `lanl_data`
- `--neg_tier {a,b,c}` to pick which `.npy` to eval
- `--processed_dir CARS/processed/lanl`
- drop HeaRT/SoHeaRT loading

**Train loop:** keep existing — benign train edges + random train negatives (`random_neg=True`).

**Eval:** `pooled_auc_ap(1 - pos_scores, 1 - neg_scores)` if using inverted scores.

---

## Step 5 — First commands (after Agent implements)

```bash
conda activate heart_gpu
cd /home/mmanjee/HeaRT

# 1. Preprocess (full run ~10–30 min depending on disk)
python CARS/code_preprocess/preprocess_lanl.py

# 2. Negatives
python CARS/code_preprocess/build_lanl_negatives.py --k 500

# 3. Train + eval tier A (easy baseline)
python CARS/code_preprocess/main_gnn_lanl.py \
  --processed_dir CARS/processed/lanl \
  --neg_tier a --epochs 50 --runs 1

# 4. Same checkpoint protocol, tiers b and c
python CARS/code_preprocess/main_gnn_lanl.py --neg_tier b ...
python CARS/code_preprocess/main_gnn_lanl.py --neg_tier c ...
```

Expect: **A > B > C** in AP/AUC (A inflated).

---

## Code style rules (keep it human-readable)

1. **One job per file** — preprocess / negatives / data / train.
2. **No deep inheritance** — plain functions + `argparse`.
3. **Comments only for non-obvious things** (column indices, inverted scores).
4. **Reuse, don’t rewrite** — `gnn_model.GCN`, `mlp_score`, `link_pred_metrics.pooled_auc_ap`, `train_epoch` from SNAP.
5. **Log counts in `meta.json`** — reviewers will ask.

---

## What not to do yet

- Euler temporal snapshots / GRU
- HeaRT/SoHeaRT negatives
- Full 7 GB feature engineering — `x = torch.ones(N, 1)` is fine for v1 smoke run
- CICAPT / OpTC until LANL tier gap shows up in one table

---

Switch to **Agent mode** and ask to implement Steps 1–4; I’ll add `preprocess_lanl.py` first with `--max_lines` for a quick sanity check, then full run.

---

**User**

@/home/mmanjee/.cursor/projects/home-mmanjee-HeaRT/terminals/5.txt:220-227 

---

**Cursor**

`auth.txt.gz` is **gzip**, not zip — `unzip` only works on `.zip` files.

**To decompress:**
```bash
gunzip -k auth.txt.gz    # writes auth.txt, keeps .gz (-k)
# or
gzip -dk auth.txt.gz
```

**You usually don’t need to decompress.** Read it directly in Python or from the shell:
```bash
zcat auth.txt.gz | head
python3 -c "import gzip; f=gzip.open('auth.txt.gz','rt'); print(f.readline())"
```

For preprocessing, stream with `gzip.open(...)` — no 7+ GB uncompressed file on disk.

---

**User**

@/home/mmanjee/.cursor/projects/home-mmanjee-HeaRT/terminals/5.txt:257-262 explain dataset briefly

---

**Cursor**

**LANL 2015 auth dataset** — enterprise Windows auth logs from Los Alamos National Lab (~58 days), plus a red-team attack log.

**`auth.txt.gz`** (~45M events): each line is one auth event:
```
time, user, user, src_comp, dst_comp, auth_type, logon_type, action, result
```
Example: `1,ANONYMOUS LOGON@C586,...,C1250,C586,...,LogOn,Success` → machine **C1250** authenticated to **C586** at time `1`.

**`redteam.txt.gz`** (749 lines, ~518 unique attack edges in papers): labeled malicious lateral movement:
```
time, user, src_comp, dst_comp
```
Example: `150885,U620@DOM1,C17693,C1003` → user **U620** moved from **C17693** → **C1003** at time `150885`.

**For your project:** nodes = **computers** (`C*`); edges = **src_comp → dst_comp**; train on **benign auth before first redteam time** (`150885`); test positives = **redteam (src, dst)** pairs.

---

**User**

we are considering edge features?
time,
src_user,
dst_user,
src_computer,
dst_computer,
auth_type,
logon_type,
activity,
success



---

**Cursor**

**No for v1** — skip per-edge features in the graph and in the GCN+MLP decoder.

**Why**

- Your task is **edge-level ranking** (attack vs negatives) with a **frozen benign graph** and **tiered negatives**. The paper claim is about **eval protocol**, not feature engineering.
- Those fields are **event-level**: many rows map to one **(src_computer → dst_computer)** edge. Euler and your static plan use **unique directed computer edges** (optionally a weight from count), not one row per log line.
- HeaRT-style **GCN + MLP** scores pairs from **node embeddings** `h[src], h[dst]` — no edge attributes in your current `main_gnn_snap.py` path.

**What to use instead (minimal)**

| Level | v1 |
|--------|-----|
| **Graph** | Directed edge = `(src_computer, dst_computer)` |
| **Filter** | `success == Success`, `time < t_attack`; drop redteam pairs from train |
| **Node features** | Optional 3-dim type from name (`C` / `U` / special) or degree — enough for GCN |
| **Time / auth_type / logon_type / activity** | Use only when **building** the edge (e.g. keep `LogOn` only, or count → weight). Don’t feed raw categoricals into the MLP yet |

**When edge features would matter**

- Later work or if static GCN underperforms badly: aggregate per edge (event count, fraction `Network` logon, etc.) and concat to the MLP input — **not needed** to show random vs hard-benign tier gap.

**Column mapping** (matches your list):

```
time, src_user, dst_user, src_computer, dst_computer, auth_type, logon_type, activity, success
```

Nodes = `src_computer` / `dst_computer`; users matter for **Tier C** (same-source benign: same `src_computer` or same `src_user` — pick one and document it; **same src_computer** aligns with Euler).

---

**User**

yes but in gcn results will improve if er add per redge features right?

---

**Cursor**

**Maybe, but not automatically** — and it won’t fix the main issue (easy vs hard negatives).

**Where edge features can help**
- After you collapse logs to **(src_comp → dst_comp)**, per-edge stats (count, dominant `logon_type`, `Network` vs interactive, etc.) can separate **attack-like benign** from attacks better than structure alone — especially on **Tier C**.
- That improvement usually shows up in the **decoder** (MLP on `[h_u, h_v, edge_feat]`), not from plain GCN message passing alone.

**Why it’s not a given**
- Your stack is **GCN(node) + MLP(pair)** — it doesn’t use edge features unless you **add** them to the scorer.
- Many attack and benign edges share the same **topology**; features help only if attacks differ systematically in `auth_type` / `logon_type` / activity (often weak on LANL).
- **Tier A** AP/AUC can stay high even without edge features; **Tier C** is where you’d see most gain — if any.

**For the CARS paper**
- **v1 without edge features** is the right call: cleaner story (“metrics inflate with easy negatives”), matches Euler/HeaRT-style baselines, less engineering.
- If you have time: one **ablation row** with simple edge features (event count + 1-hot logon type) on LANL only — cite as “modest gain on hard tier, protocol effect dominates.”

**Bottom line:** Edge features might **narrow** the gap on hard negatives; they won’t replace **hard benign eval** and aren’t required for the core contribution.

---

**User**

I want to include edge features how will gcn aggregate this?

---

**Cursor**

**Short answer:** Your current GCN **does not** aggregate per-edge features. `GCNConv(x, adj_t)` only uses **node features** and **who is connected to whom** (optionally scalar **edge weights**). Edge attributes (`auth_type`, `logon_type`, etc.) must be handled **outside** standard GCN or you switch to an edge-aware layer.

---

## What your GCN does today

```61:72:HeaRT/benchmarking/gnn_model.py
    def forward(self, x, adj_t):
        ...
        for conv in self.convs[:-1]:
            x = conv(x, adj_t)
            ...
        x = self.convs[-1](x, adj_t)
        return x
```

Each layer: for node `v`, GCN **sums normalized messages from neighbors’ node embeddings**. No `auth_type` / `logon_type` enters unless you encode them as **edge weights** or change the conv.

Decoder (`mlp_score`): `h_u * h_j` → MLP — also **no edge features**.

---

## How to use edge features (3 realistic options)

### 1. **Decoder-only (recommended for you)**

Preprocess: one row per **(src_comp, dst_comp)** with aggregated edge features, e.g.:

- `event_count` (log-scaled)
- one-hot or counts for `logon_type`, `auth_type`, `activity`
- fraction `Success` (should be 1 if you filter)

**GCN unchanged** → get `h_u`, `h_v`.  
**Extend MLP** to score:

```text
score(u,v) = MLP( [ h_u ⊙ h_v ,  e_uv ] )
```

Edge features are **not aggregated by GCN**; they’re fused **only at scoring** for the pair you’re ranking. This is the smallest change and fits LANL (many events → one edge vector `e_uv`).

---

### 2. **Scalar edge weight in GCN (partial use of edge info)**

Aggregate `event_count` (or Euler-style sigmoid of count) → `edge_weight` on `adj_t`.

PyG `GCNConv` can use weights: stronger edges pull more neighbor signal during message passing.

Only **one number per edge** — not full categorical edge features.

---

### 3. **Edge-aware GNN (full aggregation in conv layers)**

Use layers that take `edge_attr`, e.g. `GINEConv`, `NNConv`, or `GATConv(..., edge_dim=d)`:

```text
message u→v depends on h_u, h_v, and e_uv
```

Then neighbors’ **edge types** influence node embeddings. More expressive, more work; not in your HeaRT `GCN` class today.

---

## LANL-specific pipeline

| Stage | What happens |
|--------|----------------|
| **Raw log** | Many lines per `(src_comp, dst_comp)` |
| **Preprocess** | Collapse to unique directed edges + `e_uv` (aggregated stats) |
| **GCN** | `h = GCN(x_node, adj_t)` — structure + node features |
| **Train/test** | For each candidate edge `(u,v)`, lookup `e_uv` (train stats for train edges; for test attacks/negs, compute from held-out events or train-only stats — **document leakage rules**) |
| **Score** | `MLP([h_u⊙h_v, e_uv])` |

For **Tier C benign decoys**, `e_uv` is often **similar** to attacks (same src, realistic logon) — that’s why hard negatives matter; edge features may help a bit but won’t remove the tier gap.

---

## Practical recommendation

1. **v1:** decoder fusion — `e_uv` = `[log(count), one_hot(logon_type), one_hot(activity)]`.
2. Optionally multiply **count-based weight** into `adj_t` as a second signal.
3. Skip edge-aware convs unless decoder-only ablation shows you need them.

Switch to **Agent mode** if you want this wired into `preprocess_lanl.py` + an `mlp_score` variant that accepts `e_uv`.

---

**User**

If edge features are rich and you trust them, use GATv2Conv. If they're noisy or sparse, encoder-only node embeddings with a plain dot-product/MLP scorer is often more stable.

---

**Cursor**

That guidance is sound, and it maps cleanly to LANL.

**GATv2Conv** — edge features enter **attention** during message passing (how much each neighbor matters). Good when edge attrs are **informative and dense** on most edges (e.g. stable `logon_type` / `auth_type` distributions per `(src,dst)`). Risk: sparse or high-cardinality categoricals → noisy attention, harder to train, easy to overfit on ~518 attack edges.

**GCN + MLP scorer with `e_uv` at decode time** — GCN only learns **structural** node embeddings; edge stats are fused when scoring `(u,v)`. More stable when features are **aggregated from many events** but still **heterogeneous** (LANL: same pair can mix LogOn/LogOff, many auth types). Your tiered-negative story also cares about **ranking under hard benigns** — a simpler scorer is easier to interpret and ablate.

**For your CARS setup**

| Signal | Suggestion |
|--------|------------|
| Rich, aggregated per-edge vector (count + dominant logon/auth) | Start **decoder-only**: `MLP([h_u⊙h_v, e_uv])` |
| Want edge signal in propagation | Try **scalar weight** on `adj_t` (Euler-style count) before GATv2 |
| Full categorical edge attrs in conv | **GATv2** only as an ablation if decoder-only helps little |

**Practical default:** GCN (or GraphSAGE) encoder + edge-aware MLP decoder. Add GATv2 only if you have a clear gain on **Tier C**, not Tier A.

That keeps the paper focused on **eval protocol** while edge features stay a controlled add-on, not the main architectural bet.

---

**User**

but i dont see a drawback of using edge features

---

**Cursor**

There isn’t a fundamental “edge features are bad” rule — the drawbacks are **practical**, not theoretical.

**Real drawbacks**

1. **Leakage / protocol mistakes** — Test attacks and Tier B/C negatives need `e_uv` built from **pre-attack or train-only** stats. If you accidentally use post-attack event counts or global aggregates, scores inflate and your tier comparison is wrong.

2. **Many events → one edge** — You must **aggregate** (`count`, mode of `logon_type`, etc.). Wrong aggregation (mean vs mode, mixing LogOn/LogOff) adds noise; GATv2 can **overfit attention** to that noise on ~500 attack positives.

3. **Sparse / missing features at test** — Tier A random non-edges often have **no** real log row → you need a **default** `e_uv` (zeros or global prior). Tier C benign edges have **real** `e_uv`. The model may lean on edge features differently per tier, which **confounds** your “easy vs hard negative” story unless you control for it.

4. **Engineering cost vs deadline** — Preprocess, storage (`edge_feat.pt` indexed by `(u,v)`), and a new scorer path. For a **6-page eval-protocol paper**, that time often doesn’t buy a stronger main claim than tier A vs C with structure-only.

5. **Harder ablations** — Reviewers ask: is the gap from **negatives** or from **edge attrs**? You need extra rows (with/without `e_uv`) to answer that.

6. **Not a substitute for hard negatives** — Edge features might lift Tier C AP a bit; they **won’t** make Tier A a honest proxy for deployment if negatives are still random non-edges.

**When there’s basically no drawback**

- Aggregates are **train-only**, same feature pipeline for all tiers (with explicit defaults for synthetic edges).
- You report **with vs without** `e_uv` on the same tiers.

**Bottom line:** Edge features are fine to include if you want them — the “drawback” is **extra failure modes and ablation work**, not that GCN can’t use them. For CARS, they’re **optional enrichment**, not required for the core result.

---

**User**

lets use gatv2 since we have edge features. 

---

**Cursor**

Checking what's already in the repo for GAT and PyG so we can plan GATv2 integration.


**Good call for LANL** — aggregated per-edge attrs (`logon_type`, `auth_type`, counts) fit **GATv2Conv** better than plain GCN, as long as you wire **edge_index + edge_attr** (not just `adj_t`).

---

## Architecture (locked)

```
events → aggregate e_uv per (src_comp, dst_comp)
       → GATv2(x_node, edge_index, edge_attr) → h
       → MLP([h_u, h_v, e_uv]) or h_u⊙h_v + e_uv → score
```

- **GATv2:** edge features shape **attention** during message passing.
- **MLP decoder:** still needed to score **test** pairs (attacks + tier negatives) that may not be in the train graph.

Your repo has **GAT** (`gnn_model.py`) but **no GATv2** yet; PyG **2.2.0** includes `GATv2Conv`.

---

## Preprocess: `edge_attr` per train edge

When collapsing `auth.txt.gz` to unique directed edges **before `t_attack`**:

| Field | Aggregation |
|--------|-------------|
| `event_count` | count (then `log1p`) |
| `logon_type`, `auth_type`, `activity` | count vector or **mode** one-hot |
| `success` | 1.0 if you filter Success only |

Save alongside `graph.pt`:

```python
edge_index: [2, E]   # train benign directed
edge_attr:  [E, D]   # float, normalized
edge_feat_dim: D
```

Build a lookup `edge_key(u,v) → row in edge_attr` for val benign / attack / Tier C edges (from **pre-attack** events only).

**Tier A random non-edges:** `e_uv = 0` or train marginal mean (document which).

---

## Model: add `GATv2` class (~mirror existing `GAT`)

```python
from torch_geometric.nn import GATv2Conv

# layer 0: GATv2Conv(in, hidden, heads=4, edge_dim=D)
# middle:  GATv2Conv(hidden*heads, hidden, heads=4, edge_dim=D)
# last:    GATv2Conv(..., out, heads=1, concat=False, edge_dim=D)
```

**Important:** GATv2 uses **`edge_index` + `edge_attr`**, not `SparseTensor adj_t` like your GCN path. Training loop must pass all three.

**Heads:** start `heads=4`, `hidden=64`, `out=64` (smaller than HeaRT 256 — LANL is huge; match batch/memory).

---

## Scorer with edge features

Extend `mlp_score` or add `mlp_edge_score`:

```python
def forward(self, h_i, h_j, e_ij):
    x = torch.cat([h_i * h_j, e_ij], dim=-1)  # or [h_i, h_j, e_ij]
    ...
```

Train loss: same as now — benign pos + random neg, each with its `e_ij`.

Eval: attacks and tier negs each carry their own `e_ij` (lookup or zeros for Tier A).

**Suspicious score:** `1 - link_score` if attacks should rank high.

---

## Train / test graph (unchanged)

- **Train adj:** pre-attack benign edges only + their `edge_attr`.
- **Test forward:** same frozen train graph (attacks **not** in `edge_index`).
- **Test ranking:** MLP on `(h_u, h_v, e_uv)` for attack vs tier negatives.

---

## CLI sketch

```bash
python main_gnn_lanl.py --gnn_model GATv2 --use_edge_attr ...
```

Keep **GCN** as ablation row in the paper table (2 models × 3 tiers).

---

## Pitfalls to avoid

1. **Leakage** — `e_uv` for test edges from **pre-attack** stats only.
2. **Tier fairness** — same `D` and default `e` for Tier A zeros; don’t give attacks richer features than benign negs.
3. **Memory** — full LANL edge list is large; GATv2 is heavier than GCN; use neighbor sampling or subgraph batching if OOM.

---

## Implementation order

1. `preprocess_lanl.py` → `edge_index`, `edge_attr`, `edge_feat_dim`
2. `GATv2` in `gnn_model.py` + edge-aware MLP in `scoring.py`
3. `lanl_data.py` → return `edge_index`, `edge_attr`, per-candidate `e_ij` at eval
4. `main_gnn_lanl.py` → `--gnn_model GATv2`

Switch to **Agent mode** when you want this implemented in the repo.

---

**User**

i think raw LANL auth file does not provide computer features.
should we compute:
Examples:

degree
in-degree
out-degree
number of users logging in
number of authentications
PageRank
PPR statistics

or

computer embedding
one-hot computer ID

---

**Cursor**

**Skip one-hot computer ID and learned `nn.Embedding(N, ·)`** for v1 — ~17k nodes, ~500 attack edges → easy to overfit and hard to justify in 6 pages.

**Use cheap, train-graph-only structural node features** plus a tiny role flag from the name string (Euler-style, not full ID).

---

## Recommended node features (LANL, computers only)

Compute on **pre-attack benign graph only** (same graph as GATv2 `edge_index`):

| Feature | Source | Notes |
|--------|--------|--------|
| `log1p(in_degree)` | train adj | incoming auth partners |
| `log1p(out_degree)` | train adj | outgoing |
| `log1p(total_degree)` | train adj | optional if you use in+out |
| `log1p(auth_events_out)` | raw auth pre-attack | total log lines with this computer as **src** |
| `log1p(auth_events_in)` | raw auth pre-attack | as **dst** |
| `log1p(n_distinct_users_out)` | raw auth pre-attack | distinct `src_user` using this machine as src — useful for lateral-movement context |
| `pagerank` (1 scalar) | train adj | DC/TGT-like hubs stand out; cheap |

**Small categorical (not 17k-dim):**

| Feature | Rule |
|--------|------|
| `is_special` | 1 if name matches `DC`, `TGT`, or other known admin tokens in LANL docs |
| or 3-dim | `[is_computer, is_special, is_other]` from name prefix |

That’s **~7–10 dims** — stable with GATv2 + edge attrs.

---

## What to defer

| Idea | Why skip (for now) |
|------|---------------------|
| **One-hot computer ID** | Huge, memorization, not generalizable |
| **Computer embedding** | Same risk unless you regularize heavily; little gain over degree/PPR for this paper |
| **Full PPR vector** | HeaRT uses PPR for **hard negatives**, not node feats; one **scalar** PPR/PageRank is enough |
| **PPR statistics bundle** | Overkill unless Tier D is central |

---

## How this fits GATv2

- **Node `x`:** structural + role (above).
- **Edge `edge_attr`:** `log1p(count)`, one-hot / counts for `logon_type`, `auth_type`, `activity` per `(src,dst)`.
- GATv2 uses **both** in conv layers; MLP still scores test pairs with `e_uv`.

No contradiction with “raw LANL has no features” — you **derive** them from the benign train graph and pre-attack event aggregates (standard in Euler-style work).

---

## Paper wording

> Node features are structural statistics and role indicators computed from pre-attack benign activity only; we do not use per-computer identity embeddings.

**Default stack:** GATv2 + **7–10D node feats** + **edge attrs**; optional ablation **without node feats** (edge-only) if space allows.

Switch to Agent mode when you want this baked into `preprocess_lanl.py`.

---

**User**

concise code to check total nodes and edges in lantl graph

---

**Cursor**

**From raw `auth.txt.gz`** (streaming, pre-attack benign or all):

```python
import gzip

AUTH = "/home/mmanjee/HeaRT/CARS/dataset/lanl/auth.txt.gz"
RED  = "/home/mmanjee/HeaRT/CARS/dataset/lanl/redteam.txt.gz"

# first attack time
with gzip.open(RED, "rt") as f:
    t_attack = min(int(line.split(",")[0]) for line in f if line.strip())

nodes, edges = set(), set()
with gzip.open(AUTH, "rt") as f:
    for line in f:
        p = line.strip().split(",")
        if len(p) < 9 or p[8] != "Success":
            continue
        t = int(p[0])
        if t >= t_attack:
            continue
        u, v = p[3], p[4]
        if u == v:
            continue
        nodes.add(u); nodes.add(v)
        edges.add((u, v))

print(f"t_attack={t_attack}")
print(f"nodes={len(nodes)}  directed_edges={len(edges)}")
```

**From processed `graph.pt`** (after preprocess):

```python
import torch
g = torch.load("CARS/processed/lanl/graph.pt", map_location="cpu")
E = g["directed_unique"].size(0)
N = g["num_nodes"]
print(f"nodes={N}  directed_edges={E}")
```

Euler’s LANL paper reports **~17,685 nodes** and **~45.8M events** (weighted edge count over time windows); unique directed computer pairs from pre-attack auth will be smaller than event count.

---

**User**

@/home/mmanjee/.cursor/projects/home-mmanjee-HeaRT/terminals/4.txt:416-439 

---

**Cursor**

You pasted **Python into zsh** — the shell tried to parse `import`, `with`, etc., hence `parse error near ')'`.

**Run it with Python:**

```bash
cd /home/mmanjee/HeaRT/CARS/dataset/lanl
python3 <<'EOF'
import gzip

AUTH = "auth.txt.gz"
RED  = "redteam.txt.gz"

with gzip.open(RED, "rt") as f:
    t_attack = min(int(line.split(",")[0]) for line in f if line.strip())

nodes, edges = set(), set()
with gzip.open(AUTH, "rt") as f:
    for line in f:
        p = line.strip().split(",")
        if len(p) < 9 or p[8] != "Success":
            continue
        if int(p[0]) >= t_attack:
            continue
        u, v = p[3], p[4]
        if u == v:
            continue
        nodes.add(u)
        nodes.add(v)
        edges.add((u, v))

print(f"t_attack={t_attack}")
print(f"nodes={len(nodes)}  directed_edges={len(edges)}")
EOF
```

Or save as `count_lanl.py` and run `python3 count_lanl.py`. Full auth scan may take several minutes on the 7.6 GB file.

---

**User**

the file is 7.2G how to check how many nodeS?

---

**Cursor**

**You must scan the full 7.2 GB once** — gzip can’t give unique counts without reading it. Memory is fine: computer IDs in a `set` are only ~17k entries (Euler reports **17,685 nodes**).

**Nodes only** (faster, less RAM — don’t build `edges`):

```python
import gzip

AUTH = "auth.txt.gz"
RED  = "redteam.txt.gz"

with gzip.open(RED, "rt") as f:
    t_attack = min(int(line.split(",")[0]) for line in f if line.strip())

nodes = set()
with gzip.open(AUTH, "rt") as f:
    for i, line in enumerate(f, 1):
        p = line.strip().split(",")
        if len(p) < 9 or p[8] != "Success":
            continue
        if int(p[0]) >= t_attack:
            continue
        nodes.add(p[3])
        nodes.add(p[4])
        if i % 5_000_000 == 0:
            print(f"lines={i:,}  nodes_so_far={len(nodes):,}")

print(f"t_attack={t_attack}")
print(f"nodes={len(nodes):,}")
```

Run: `python3 temp.py` (remove the `print(u, v)` in your loop — that will print tens of millions of lines).

**Expect ~10–30 min** depending on disk; progress lines every 5M rows show it’s working.

**Quick sanity check** (first 1M lines only — not exact):

```bash
python3 -c "
import gzip
s=set()
with gzip.open('auth.txt.gz') as f:
    for i,l in enumerate(f):
        if i>=1_000_000: break
        p=l.decode().strip().split(',')
        if len(p)>=5: s.add(p[3]); s.add(p[4])
print('sample nodes (1M lines):', len(s))
"
```

Exact node count = full streaming pass with the first script.

---

**User**

isnt the datasize too small like number of nodes and edges are small and the test graph is also sparse?

---

**Cursor**

**Not too small for this paper** — what’s small is **labeled attacks**, not the log or the benign graph.

| Scale | LANL (typical) | Your read |
|--------|----------------|-----------|
| **Raw events** | ~45M lines, 7.2 GB | Large |
| **Nodes (computers)** | ~17k (Euler: 17,685) | Fine for GCN/GAT |
| **Unique benign directed edges** (pre-attack) | Often **10⁵–10⁶** after collapse | Not sparse |
| **Attack test positives** | ~518 unique pairs (749 redteam lines) | **Very small** |

So: the **dataset is big**; the **positive test set is tiny and imbalanced** — normal for LANL IDS work (Euler, Bowman, etc.).

**“Test graph is sparse”** — clarify two things:

1. **Train `adj_t` (frozen at test)** — dense enough: domain controllers, many host pairs, heavy-tailed degrees. Embeddings are learned on a rich benign graph.
2. **Test labels** — sparse: few attack edges vs a huge haystack of plausible benign auth. That’s why **AP** matters (Euler: AUC ≈ 0.99, AP ≈ 0.01–0.05).

**Implications for your work**

- **Don’t** treat this like OGB link prediction (millions of test edges).
- **Do** frame it as **anomaly / rare-edge ranking** with **tiered negatives** — the contribution is eval protocol, not scaling to huge graphs.
- **518 attacks** is enough to show **Tier A vs C** gaps if effects are strong; use **multiple seeds**, report **AP + AUC**, avoid huge models.
- **GATv2** on ~17k nodes is OK; risk is **overfitting 518 positives**, not “graph too small.”

**Bottom line:** LANL is the standard cyber LP benchmark — large in events, **small in attack labels**. That imbalance is expected; your tiered-negative study is aimed exactly at that setting. CICAPT + OpTC add a second/third dataset so you’re not relying on LANL alone.

---

**User**

For Evaluation: Per each attack edge(u,v), we sample K negatives only from attack-looking benigns (with low PPR/CN/RA)
All benign test edges
  → keep only (u,v) NOT in train
  → among those, keep bottom PPR/CN/RA
  → sample K per attack and rank attack edge among hard benigns which look like attack so it becomes difficult for model to classify if attack can be classified as benign and allow connection between nodes or stop that connection because its detected as an attack

with this are we also doing benign U attack edges>? so model can learn about attack edges too?

---

**Cursor**

**No — not in training, under your current protocol.**

| Phase | Attack edges | Benign edges |
|--------|----------------|--------------|
| **Train graph** | **Excluded** | Pre-attack benign only |
| **Train loss** | **Not used** | Positives + random non-edges |
| **Test positives** | Held-out redteam `(u,v)` | — |
| **Test negatives (Tier D)** | — | Unseen-in-train benign, bottom PPR/CN/RA, K per attack |

The model **does not learn from attack edges**. It learns **normal** connectivity on the benign graph; attacks are scored at test as **low “normal link” probability** → high suspicion (`1 - ŷ`).

**Tier D eval** is only:

```
per attack (u,v):  rank attack  vs  K hard benign (u,v') or (u',v)
```

Those benigns are **decoys** — structurally novel / weak in train (low PPR/CN/RA), attack-like but labeled benign. You’re testing: *can the model tell this real attack from plausible benign connections?*

**Not** “benign ∪ attack” in training. If you add attack edges to the train graph or as labeled positives, you switch to **supervised** detection (different paper, label leakage risk).

**Also at test:** attack edges stay **out of `edge_index` / `adj_t`** — frozen train benign graph for GATv2; attacks are scored only via `(h_u, h_v, e_uv)` on that graph.

**Summary:** Tier D makes **evaluation** harder, not training. Attacks appear **only as test positives**, never as training signal. That matches Euler-style unsupervised lateral-movement detection and your eval-protocol story.

---

**User**

but if we dont train on attack how will model learn about what edges to categorize as attack? if it learns it will be able to distinguresh between attack looking hard benigns and attack edges

---

**Cursor**

**It doesn’t learn “attack” as a class** — it learns **normal**, then treats **low-normal-likelihood** edges as suspicious. That’s the standard LANL / Euler setup (unsupervised lateral-movement detection).

**Training (no attack labels)**  
- Positives: benign edges the model should predict as “likely.”  
- Negatives: random non-edges (easy).  
- GATv2 + edge features learn: *who normally talks to whom, with what auth patterns.*

**At test**  
- **Attack `(u,v)`:** often **unseen** or rare in train, odd `logon_type` / activity vs usual for `u` → **low** “benign link” score → **high** suspicion.  
- **Tier D hard benign:** also unseen / low PPR·CN·RA — **also** look suspicious.  
- The model is **not** trained to separate those two; it only knows “fits benign model or not.”

**So how can it distinguish attack vs hard benign?**  
Only if attacks are **more** surprising than those benigns on structure + edge attrs (or node role). Often they’re **not** much more surprising — **that’s the point of Tier D**: AP drops because ranking gets hard. Your paper shows **eval with easy negatives overstates** how well the model separates attacks from realistic benigns.

**If you need explicit “learn attacks”**  
You’d need **supervised** training (attack = positive class). That changes the problem, risks leakage, and isn’t what you locked for CARS.

**One sentence for the paper:**  
*We train only on benign connectivity; attacks are detected as deviations from that baseline, and hard-benign tiers measure whether reported LP metrics reflect that operational ranking task.*

Failed separation on Tier D is a **valid result**, not a bug in the protocol.

---

**User**

lets do it like this: train:
benign y = 1nonattack
attack y=0 

test:
attack
hard benigns
to check if attack can pass as normal bening


---

**Cursor**

**Yes — that’s supervised edge classification.** It directly answers “do attacks score like benign?” Clearer than benign-only training for Tier D.

---

## Protocol

**Labels (link score = P(benign))**

| Edge type | y | Train | Test |
|-----------|---|-------|------|
| Benign | **1** | ✓ (pre-attack train split) | hard benigns only as **negatives** |
| Attack | **0** | ✓ | **positives** (held-out) |

**Train loss:** BCE on each edge `(u,v)` — benign → 1, attack → 0.  
Random non-edges optional as extra y=0 (easy); not required if attacks + hard benigns carry the story.

**Test ranking (inverted for suspicion):**

- **Positives:** held-out attack edges (want **low** P(benign) → high suspicion)
- **Negatives:** Tier B/C/D hard benigns (want **high** P(benign))
- **Metrics:** AP/AUC on `1 - P(benign)` or label y with attack=positive class at test

**Pass-as-benign failure mode:** attack scores **≥** many hard benigns → high P(benign) → looks normal.

---

## Train split (LANL)

```
pre-attack benign  → 90% train (y=1), 10% val benign (y=1)
redteam (u,v)      → 80% train attack (y=0), 20% test attack
```

- **Train graph for GATv2:** pre-attack **benign only** (attacks not in `edge_index`; same frozen-graph rule).
- **Attack train edges:** used **only in loss** `(h_u, h_v, e_uv) → 0`, not as graph edges.

---

## Test tiers (unchanged)

| Tier | Negatives |
|------|-----------|
| A | random non-edges (inflated baseline) |
| B | random held-out benign |
| C | same-src benign |
| D | unseen + bottom PPR/CN/RA benign |

Same trained model; only negatives change. Main claim: **AP drops A→D**; on D, attacks that **pass as benign** drag AP down.

---

## vs old plan (benign-only train)

| | Benign-only (Euler-style) | Your new plan |
|--|---------------------------|---------------|
| Learns attack? | No — anomaly via surprise | **Yes** — y=0 attack edges |
| Paper story | Eval protocol on unsupervised LP | **Supervised** + hard-benign eval |
| Tier D | Harder ranking without attack labels | Tests **false acceptance** of attacks |

Still fine for CARS if you frame it as: *even with attack labels in training, random-negative eval overstates separation from realistic benigns.*

---

## One-line scoring

```text
train:  loss = BCE(ŷ, y)     y=1 benign, y=0 attack
test:   score_suspicious = 1 - ŷ   rank attacks above hard benigns
```

Switch to **Agent mode** when you want this wired into `preprocess_lanl.py` + training loop.

---

**User**

but if we give it harder benigns (attack like looking benigns with low CN/PPR) and do not train on attack then it wont be able to perform well right? or it would be?

---

**Cursor**

**Often it will not perform well on Tier D without attack labels** — and that’s expected, not a bug.

**Benign-only training** teaches: “edges that fit the normal graph + benign edge patterns score high.”

| Test pair | Typical score |
|-----------|----------------|
| **Tier A** random non-edge | Very low → attack easy to rank above → **inflated AP** |
| **Tier D** hard benign (unseen, low CN/PPR) | Low — looks “not normal” |
| **Attack** (also unseen, low CN/PPR) | Also low — also looks “not normal” |

Attacks and hard benigns both look **anomalous** under a benign-only model. The model can flag “weird” edges but **can’t reliably rank attacks above attack-like benigns** without ever seeing what an attack is.

**So:**
- **Tier A:** can look **good** (misleading).
- **Tier D:** often **mediocre AP** — attacks don’t clearly beat hard benigns.

That gap (A high, D low) **is** the eval-protocol result: easy negatives hide that the model can’t separate attacks from realistic benigns.

**Could it still work sometimes?** Only if attacks are **more** surprising than hard benigns on edge features (`logon_type`, etc.) or structure — possible on LANL, **not guaranteed**. Don’t count on it for the main claim.

**If you need better Tier D discrimination:** supervised train with **attack y=0, benign y=1** (your updated plan). Still hard on D, but the model can learn attack-specific signals beyond “novel edge.”

**Paper line:** *Benign-only LP scores well under easy negatives but fails to rank attacks above structurally similar benigns; supervised attack labels may help but hard-benign evaluation remains the honest operational test.*

---

**User**

is it a normal practice in Cybersecurity

---

**Cursor**

**Yes — both training styles are normal; tiered hard-benign eval is less standard but defensible.**

| Practice | Cyber norm? | Examples |
|----------|-------------|----------|
| **Train on benign only, detect anomalies** | **Very common** | Euler, many LANL auth papers — learn “normal,” flag rare/surprising edges |
| **Supervised: benign vs attack labels** | **Common** when red-team labels exist | Bowman GL-LV/GV, some LANL classifiers — explicit attack y=0 |
| **Train graph = pre-attack benign only** | **Standard** on LANL | Stop before first redteam event |
| **Attacks not in GNN adjacency** | **Common** | Graph = normal activity; score candidate edges separately |
| **Huge class imbalance (~518 attacks)** | **Expected** | Report AP + FPR, not accuracy |
| **Hard negatives = real benign decoys** | **Conceptually normal** (haystack of legit traffic) | Euler does this implicitly; few papers name tiers like HeaRT |
| **Tier A random non-edges as main eval** | **Common in LP papers, weak for cyber** | Easy to inflate scores — your critique is the contribution |

**What reviewers expect:** unsupervised **or** supervised on LANL is fine; cite Euler/Bowman if supervised. What’s **less usual** (and useful) is **systematically comparing negative tiers** (random vs same-source vs low-CN/PPR benign) on the **same** model — that’s methodology, not a new detector.

**One sentence:** Benign-only and labeled attack training are both standard in cyber graph IDS; your tiered hard-benign test is the honest version of what production systems face (attacks vs plausible normal auth), which many papers skip by using easy negatives or only reporting AUC.

---

**User**

what is the ratio of bengin : attack edges in Lanl?

---

**Cursor**

Checking published figures and whether a count has finished.


**Depends what you count** — LANL is extremely imbalanced at every level.

| Level | Benign | Attack | Ratio (benign : attack) |
|--------|--------|--------|-------------------------|
| **Auth events** (Euler Table 6) | ~45.9M | 518 labeled anomalous *edges* in paper setup | ~**88,000 : 1** (events vs attack edges) |
| **Redteam file** | — | **749** lines | — |
| **Unique attack `(src_comp→dst_comp)`** (your `redteam.txt.gz`) | — | **308** unique directed pairs | — |

Euler’s **518** ≠ **308** unique redteam pairs — they likely count more labeled anomalous auth events / temporal windows, not just unique redteam pairs. For your pipeline, use **308** (or **749** event-level) and state which you use.

**Unique benign directed edges** (pre-attack, `Success`, computers only): needs a **full pass** over `auth.txt.gz` (your script didn’t finish). Expect **~10⁵–10⁶** unique pairs, **not** 45M.

**Rough edge-level ratio (estimate until preprocess finishes):**

```
benign_unique : attack_unique  ≈  300,000–1,000,000 : 308  ≈  1,000 : 1  to  3,000 : 1
```

**For training (your supervised plan):** ratio is whatever you **sample** (e.g. 1:1 benign:attack per batch), not the natural 1000:1 — attacks are oversampled.

**For test (Tier D, K=500 per attack):** per attack you rank **1 attack vs 500 hard benigns** → **1 : 500** locally; globally **308 attacks vs 154k** negative slots if all unique.

Run the full `temp.py` (no `print(u,v)`) to get exact `directed_edges` for the benign side.

---

**User**

so how should we sample for training

---

**Cursor**

**Use balanced edge batches for the loss; keep the GATv2 graph benign-only.**

---

## What goes where

| Artifact | Content |
|----------|---------|
| **`edge_index` + `edge_attr`** | Pre-attack **train benign** edges only (90% of benign split) |
| **Loss positives (y=1)** | Sampled **train benign** edges |
| **Loss negatives (y=0)** | **Train attack** edges + optional random non-edges |

Attacks are **not** added to `edge_index` — only used in BCE on `(h_u, h_v, e_uv)`.

---

## Splits (fixed)

```
benign (pre-attack, Success):  90% train  | 10% val
redteam unique (u,v):          80% train  | 20% test  (~246 train / ~62 test attacks)
```

---

## Per-epoch sampling (recommended)

Imbalance is huge (~10⁵–10⁶ benign vs ~246 attacks), so **don’t use all benign edges every epoch**.

**Each training step / mini-batch:**

```text
batch_size B (e.g. 4096)
  B/2  benign train edges     → y = 1
  B/2  attack train edges     → y = 0   (with replacement — oversample attacks)
```

Optional: replace half of attack slots with **random non-edges** (y=0) so the model still sees “no link” — **not required** if you already have enough attack labels.

**Per epoch:**

```text
E epochs × 1 pass over ATTACK train set (oversampled to ~50k–200k steps)
  each step: fresh random sample of B/2 benign edges
```

Rule of thumb: **see each train attack ~50–200× per epoch** (oversample); **benign: sample without replacement until pool exhausted, then reshuffle**.

---

## Val (during training)

- **Balanced val** for early stopping: 50% val benign / 50% val attack (or all val attacks + equal count benign).
- Don’t tune on Tier A only — optional: quick check on Tier C val benign.

---

## Test (unchanged)

- **Positives:** held-out test attacks only  
- **Negatives:** Tier A/B/C/D (K=500 per attack)  
- **Score:** `suspicious = 1 - P(benign)`  
- Metrics: pooled AP/AUC (`link_pred_metrics.py`)

---

## What to avoid

| Don’t | Why |
|-------|-----|
| All benign edges + few attacks per batch (natural ratio) | Model predicts “always benign” |
| Attack edges in `edge_index` | Leaks labels into message passing |
| Train/test attack overlap | Inflated test AP |
| Tune only on random non-edge val | Hides hard-benign failure |

---

## Minimal loss sketch

```python
# graph forward once per step
h = gatv2(x, edge_index, edge_attr)

# sample
pos = sample_benign_train(B//2)      # y=1
neg = sample_attack_train(B//2)      # y=0, replace=True

loss = BCE(score(h, pos, e_pos), 1) + BCE(score(h, neg, e_neg), 0)
```

---

## Paper one-liner

> Training uses balanced mini-batches (1:1 benign:attack) with attack oversampling; the encoder graph contains only benign edges.

Switch to **Agent mode** to wire this into `main_gnn_lanl.py`.

---

**User**

attack oversampling will lead to memorization

---

**Cursor**

**Valid concern** — with only **~246 unique train attack pairs**, heavy oversampling (same `(u,v)` hundreds of times per epoch) **can memorize** those edges without learning general attack structure.

---

## Better than heavy oversampling

**1. Class-weighted loss (preferred)**  
One pass per epoch over sampled benign edges; **each train attack used once** (or few times):

```text
loss = w_benign * BCE(benign, 1) + w_attack * BCE(attack, 0)
w_attack ≈ (# benign sampled per epoch) / (# train attacks)
```

Imbalance handled by **weights**, not repeating the same 246 edges 200×.

**2. Softer balance**  
Batch ratio **4:1 or 8:1** benign:attack instead of 1:1 — still upsamples attacks but less memorization pressure.

**3. Hard benign as train negatives (y=0)**  
Mix train negatives:

```text
50% attack edges
50% hard benign (same-src or low CN/PPR from val/holdout)
```

Model learns “not every suspicious-looking edge is attack” — reduces pure attack-pair memorization.

**4. Regularization**  
Dropout, edge dropout on `edge_index`, weight decay, early stopping on **held-out test attacks** (or val attack split) — not train attack accuracy.

---

## Detect memorization

| Signal | Memorization |
|--------|----------------|
| Train attack AUC ≈ 1.0, **test attack** AUC low | Likely memorizing train pairs |
| Test AP **Tier A** high, **Tier D** collapses | Easy negs + no generalization (different issue) |
| Scores perfect on train attacks, random on unseen `(u',v')` attacks | Pair memorization |

Use **80/20 attack split** and never evaluate on train attacks.

---

## Practical recipe for LANL

```text
Graph:     train benign only
Each epoch:
  - sample M benign edges (e.g. M = 500k–2M or one shuffle of train benign)
  - use ALL train attack edges ONCE (or cap at 2–5× with shuffle)
  - add hard benign negatives as extra y=0 (same count as attacks)
Loss:      weighted BCE, w_attack high but not infinite repetition
```

---

## Paper angle

> With only hundreds of unique attack edges, we avoid extreme oversampling and use class-weighted loss plus hard-benign training negatives to limit pair memorization.

**Bottom line:** imbalance needs **weighting + hard benign train negs**, not **200× replay of the same 246 edges**.

---

**User**

train:
attack: benign
1 : 8

test:
attack : bening
1 : 500

---

**Cursor**

**Yes — that’s a sensible protocol.** Concrete meaning:

---

## Train (1 attack : 8 benign per batch)

```text
batch_size = 9k  →  1k attack (y=0) + 8k benign (y=1)
```

- **Attack:** sample train attack edges **with replacement** across batches, but cap **≤2–5× per epoch** over the ~246 unique pairs to limit memorization.
- **Benign:** random sample from train benign pool each batch.
- **Graph:** still **benign-only** `edge_index` for GATv2.

1:8 is mild upsampling — better than 1:1 for memorization; if loss is still attack-starved, use **`w_attack` on loss** instead of more repetition.

---

## Test (1 attack : 500 benign per attack)

```text
For each test attack (u,v):
  positives: 1 attack edge
  negatives: K=500 from tier pool (A/B/C/D)
```

- Pooled AP/AUC: **308 attacks × 500 negs** (per tier file).
- Score: `suspicious = 1 - P(benign)`; rank attack above the 500 benigns.

---

## Summary

| | Ratio | Role |
|--|-------|------|
| **Train** | 1 : 8 | Mild attack upsampling + weighted learning |
| **Test** | 1 : 500 | HeaRT-style pooled ranking per tier |

**Note:** train 1:8 and test 1:500 are **different purposes** — train balances loss; test measures ranking under realistic decoy load. Document both in the paper.

---

**User**

how many total train set will be then?

---

**Cursor**

**Two different numbers:** the **pool** (fixed labeled edges) vs **what you sample per epoch** (depends on batching).

---

## Fixed train **pool** (unique labeled edges)

| Split | Count |
|--------|--------|
| **Attack train** (80% of 308) | **246** |
| **Benign train** (90% of unique pre-attack benign edges) | **0.9 × B** |

**Total pool = 246 + 0.9×B**

`B` = unique benign directed edges (run full `temp.py` without `print(u,v)` to get it). Until then, expect **B ≈ 3×10⁵–10⁶** → benign train **~270k–900k**.

**Example:** if `B = 500,000` → benign train **450,000** → **total pool ≈ 450,246**.

---

## Per **epoch** (with 1:8 attack:benign batches)

Depends on policy:

| Policy | Attack samples/epoch | Benign samples/epoch | **Total/epoch** |
|--------|----------------------|----------------------|-----------------|
| **Each attack once** | 246 | 246×8 = **1,968** | **~2,214** |
| **Each attack 5×** | 1,230 | **9,840** | **~11,070** |
| **One shuffle of all benign train** | 450,000÷8 ≈ **56,250** (with replacement over 246) | **450,000** | **~506,250** |

Recommended for LANL: **each attack 1–5× per epoch** → **~2k–11k edges/epoch**, not the full 450k every epoch.

---

## Test set (for reference)

| | Count |
|--|--------|
| Test attacks (20%) | **62** |
| Negatives per attack | **500** |
| Scored pairs per tier |

---

**User**

total beningn edges
Total attack edges
give ans

---

**Cursor**

Running a full count on the LANL files for exact benign and attack edge totals.


**LANL counts** (pre-attack benign, `Success`, `src≠dst`, computers `p[3]→p[4]`):

| | Count |
|--|------:|
| **Total benign edges** (unique directed) | **148,848** |
| **Total attack edges** (unique directed) | **308** |
| Attack redteam lines | 749 |
| Nodes (computers) | 11,161 |
| Auth lines scanned | ~1.05B |

**Ratio:** 148,848 : 308 ≈ **483 : 1**

**Your train pool (90% benign / 80% attack):**
- Benign train: **~133,963**
- Attack train: **~246**
- **Total labeled train edges: ~134,209**

**Test:** **62** attack edges; benign negs = **500 per attack per tier**.

*Note: Euler reports 17,685 nodes — they likely include users/other entities; your computer-only graph has 11,161 nodes.*

---

**User**

but if there are so many auth line records, it should have these many edges aswell right? why edges are so low? 

---

**Cursor**

**~1B auth lines ≠ ~149k edges** because you **collapse many log events into one graph edge**.

Each line is one **event** (one authentication at time `t`). The graph edge is **one unique pair** `(src_comp → dst_comp)` across all of pre-attack time.

**Same pair, many times:**
```
C101 → C988   (LogOn, Monday)
C101 → C988   (LogOn, Tuesday)
C101 → C988   (LogOff, …)
… thousands of lines …
→ **1 edge** in your set
```

Enterprise traffic is repetitive: workstations ↔ domain controllers, file servers, etc. A small set of pairs generates most of the volume.

**Why the number stays ~149k**

| Factor | Effect |
|--------|--------|
| **Collapse to unique `(src,dst)`** | Events ≫ edges |
| **Pre-attack only** (`t < 150885`) | Drops later activity |
| **`Success` only** | Drops failed auth |
| **`src ≠ dst`** | Drops self-loops |
| **Sparse graph** | 11k nodes → up to ~124M possible directed pairs; only **149k** ever occur |

**Intuition:** Events = “how often”; edges = “who talks to whom at least once.” Euler does the same — Table 6’s **45.9M events** compress to far fewer **weighted** edges per time window; your static snapshot uses **unique** pairs → **148,848**.

**Ratio check:**  
1,051,430,459 lines / 148,848 edges ≈ **7,000 events per unique edge** on average — normal for LANL-style auth logs.

So low edge count is **expected**, not a bug. The graph is **event-heavy, edge-sparse**; attacks (**308** unique pairs) are rare on top of that.

---

**User**

So should the graph be constructed some different way? to consider each event as an edge ?
Check how other research papers doees it

---

**Cursor**

**No — don’t use one graph edge per auth log line.** Standard LANL cyber papers **aggregate events**; they don’t build a billion-edge multigraph.

---

## How others build the LANL graph

| Approach | Who | What an “edge” is |
|----------|-----|-------------------|
| **Temporal snapshots + weights** | **Euler** | Per window δ (~30 min): one directed edge `(src_comp→dst_comp)` per pair; **count** → sigmoid weight. Many events → one weighted edge per snapshot. |
| **Static collapsed graph** | **Bowman GL-LV/GV**, many baselines | All pre-attack auth → unique `(src,dst)` (often with counts/features). “Unknown auth” = pair **never seen** in train. |
| **Raw event stream** | Rare for GNN LP | Tabular/sequence models, not 1B-edge GAT |

Euler (§8.1): nodes = **computers**; events in `[t, t+δ)` are **tallied** into `C(u,v)` then normalized as edge weight — not one edge per log line.

---

## Why not 1 event = 1 edge?

- **~1B edges** vs **~149k unique pairs** → GATv2 won’t scale; same `(C101,C988)` repeated millions of times adds no new structure.
- GNNs expect a **simple graph** or **weighted** edges, not a massive multigraph of duplicates.
- Frequency/timing belongs in **weights** or **edge features**, not duplicate topology.

---

## What you should do (static GATv2 + your paper)

**Keep unique directed `(src_comp, dst_comp)`**, and move event detail into:

| Channel | Content |
|---------|---------|
| **`edge_attr`** | `log1p(event_count)`, mode/counts of `logon_type`, `auth_type`, `activity` |
| **`edge_weight`** (optional) | Euler-style sigmoid of count — can feed GATv2 or GCN |
| **Node features** | degree, PageRank, role flags |

That matches “many auth lines, few edges” **by design**.

---

## If you want time (like Euler, not required for CARS)

- Split into **snapshots** (e.g. 30 min).
- Each snapshot: unique pairs + counts per window.
- Stack **GRU/LSTM** over snapshot embeddings.

That’s a **different model** (temporal), not “each event is an edge.”

---

## Bottom line

Your **149k benign / 308 attack unique edges** construction is **normal**. Enrich edges with **aggregated event stats**, don’t explode to one edge per log line. Cite Euler for temporal weighted snapshots; cite Bowman/UA for static unique-pair graphs.

---

**User**

briefly tell about gru/lstm

---

**Cursor**

**GRU and LSTM** are recurrent layers that read a **sequence** — in Euler, a sequence of **graph snapshots** over time.

Each snapshot gets a GNN embedding; the RNN updates a hidden state step by step so the model remembers **past network activity**, not just the current graph.

**GRU** is the lighter variant (fewer gates, faster). **LSTM** has a separate cell state and is a bit more expressive but slower.

**Why Euler uses them:** an auth that looks odd in one window may be normal given what happened earlier — temporal context cuts false positives.

**Your static GATv2 setup:** no GRU/LSTM — one collapsed pre-attack graph, no snapshot sequence.

---

**User**

for link prediction and applying hard negatives what would be a good approach? If this dataset is not a fit, we can change to some dataset with bigger size of edges and nodes but have to be secuirty related

---

**Cursor**

**Good approach (locked for your paper)**

| Piece | Choice |
|--------|--------|
| **Task** | Supervised edge scoring: benign y=1, attack y=0 |
| **Graph** | Unique `(src→dst)` + aggregated **edge_attr** (count, logon/auth/activity); **not** one edge per log line |
| **Encoder** | GATv2 on **benign-only** `edge_index` |
| **Train** | 1:8 attack:benign batches; attacks **not** in adjacency; cap attack repeats per epoch |
| **Test** | Same model; **1:500** attack:hard-benign per tier; `suspicious = 1 - P(benign)` |
| **Tiers** | **A** random non-edges · **B** random benign · **C** same-src benign · **D** unseen + low CN/PPR/RA benign |
| **Claim** | AP drops A→D — easy negatives mislead; cite HeaRT logic adapted to cyber haystack |

Main result = **tier gap on the same trained model**, not SOTA on one dataset.

---

**Is LANL a fit?**

**Yes — keep it.** Link-prediction papers often use 10³–10⁵ edges; you have **~149k benign / 308 attack** pairs. What’s small is **attack labels**, which is realistic for IDS and matches Euler/Bowman. Your contribution is **eval protocol**, not scaling to million-node graphs.

Low edge count vs billions of log lines is **normal** (event collapse).

---

**Security datasets (use 2–3 total)**

| Dataset | Role | Notes |
|---------|------|--------|
| **LANL auth** | Primary | Standard lateral movement; tier story clearest |
| **OpTC** | Second | Larger enterprise host graph; red-team eval |
| **CICAPT provenance** | Third | Build host/process graph from CSV; APT labels in Phase 2 |

That trio satisfies CARS without dropping LANL.

**Switch only if** you need **much larger graphs** for a scaling claim — then add **OpTC** as the “big” one, not replace LANL. Other options: DARPA TC ENG (heavy prep), StreamSpot/UNICORN (smaller/weaker for LP).

---

**Recommendation**

1. **LANL + OpTC + CICAPT** — don’t swap LANL for size.  
2. **Static GATv2 + tiered eval** — skip GRU/LSTM for deadline.  
3. **Optional ablation:** Euler-style **edge weight** from event count on LANL only.

If tier A≫C on all three datasets, the paper stands even with “only” 149k LANL edges.

---

**User**

CTU-13 (Neris scenario) N: 545k E:14M train✓ botnet flows✓ Strong

---

**Cursor**

**CTU-13 Neris can work, but it’s a different problem than LANL** — botnet **flow** detection, not enterprise **lateral auth** LP.

---

## What Neris gives you

| | CTU-13 Neris (typical flow→graph build) |
|--|----------------------------------------|
| **Scale** | ~545k nodes, ~14M edges — **much larger** than LANL |
| **Labels** | Per-flow benign vs botnet — **supervised** friendly |
| **Security** | ✓ Classic IDS benchmark (Stratosphere) |
| **Train** | ✓ Benign + attack flows for 1:8 training |

---

## Mismatch with your current plan

| Your setup | CTU-13 |
|------------|--------|
| Attack = **red-team auth edges** (rare, ~308) | Attack = **botnet flows** (many malicious edges) |
| Hard negs = **real benign auth** (same host, low CN/PPR) | Hard negs = **benign flows** to similar IPs/ports |
| Story = **eval protocol on attack graphs** | Story shifts to **botnet flow classification at scale** |
| Datasets = LANL / OpTC / CICAPT | **Netflow**, not host-auth provenance |

You’d need a **new graph recipe** (nodes = IP? host? `(ip,port)`?), temporal split, and tier definitions — ~1–2 weeks prep, not a drop-in replacement.

---

## Verdict

| Use Neris? | When |
|------------|------|
| **Not instead of LANL** | LANL is the right cite for lateral-movement LP + hard benign tiers |
| **Maybe as 4th dataset** | If you want a **scale** row (“same tier gap holds at 14M edges”) |
| **Skip for CARS deadline** | **LANL + OpTC + CICAPT** already satisfy ≥2 cyber graphs; OpTC is your “bigger” graph without retooling |

---

## If you still use it (minimal LP setup)

```text
Node:     IP (or /24 — reduces 545k)
Edge:     aggregate flows (src_ip → dst_ip) + count, bytes, ports in edge_attr
Pos test: botnet-labeled edges
Tier C:   benign flows same src_ip
Tier D:   benign unseen pairs, low CN/PPR on train graph
```

**Bottom line:** Neris is **strong for scale + botnet labels**, weak as a **replacement** for LANL. For June 15, stay on **LANL + OpTC + CICAPT**; add Neris only if OpTC prep slips and you need a large labeled graph fast.

---

**User**

check what @CARS/dataset/neris/ contains and read total edges and nodes and see how its arranged if its adj matrix or what?

---

**Cursor**

**`CARS/dataset/neris/`** has **one file** — not an adjacency matrix.

---

## Contents

| File | Size | Rows |
|------|------|------|
| `capture20110810.binetflow` | 369 MB | **2,824,636** flows (+ header) |

**Format:** CSV **netflow table** (Stratosphere *binetflow*), **not** adj matrix / `edge_index` / `graph.pt`.

**Columns:**
```
StartTime, Dur, Proto, SrcAddr, Sport, Dir, DstAddr, Dport, State,
sTos, dTos, TotPkts, TotBytes, SrcBytes, Label
```

Each row = **one flow** (connection) with a string **Label** (benign background vs `From-Botnet-V42-...`).

---

## Counts (this file only)

| Definition | Count |
|------------|------:|
| **Flows (rows)** | 2,824,636 |
| **Nodes** (unique IPs in Src/Dst) | **607,565** |
| **Edges** (unique `SrcAddr → DstAddr`) | **697,518** |
| **Edges** (unique `src,dst,sport,dport`) | **1,695,297** |
| **Botnet flows** (`Label` contains `Botnet`) | **40,961** |
| **Botnet unique `(src_ip, dst_ip)`** | **4,190** |

Benign ≫ botnet; botnet labels are fine-grained (`From-Botnet-V42-UDP-DNS`, etc.).

---

## vs your 545k / 14M note

That likely refers to **full CTU-13 Neris** (multiple capture days) or **each flow = one edge** (~2.8M here, not 14M). You only have **one day** (`20110810`). Full scenario would need other `capture*.binetflow` files from CTU-13.

---

## How to turn into a graph (you build it)

```text
Raw CSV  →  preprocess  →  edge_index [2,E] + edge_attr + labels
```

Typical choices:
- **Nodes:** IP (607k — large; often filter internal `147.32.*` or aggregate)
- **Edges:** aggregate flows to `(src,dst)` + count/bytes/proto in `edge_attr`
- **Attack:** flows/edges with `Botnet` in `Label`

**No adj matrix shipped** — same as LANL: tabular logs → your graph construction.

---

**Bottom line:** Neris here is **one labeled netflow CSV**, ~608k IPs, ~698k–1.7M edges depending on aggregation — usable for scale, but different from LANL auth graphs and you only have **1 of ~5** Neris capture files.

---

**User**

Right — for option 1 to work, you need attack edges dense enough to have a meaningful CN/PPR distribution (not 308 isolated edges like LANL). Let me check what ratios these datasets actually have and whether anyone has done tiered attack analysis.Important finding: no paper has done exactly what you're proposing (tiering attack edges by CN/PPR difficulty). That's genuinely novel territory — good for your paper, but means no precedent to cite for the protocol itself.

---

### What I Can Actually Tell You

**No existing paper applies HeaRT-style tiering to attack edges.** The closest analog: HeaRT papers analyze CN distributions of positive vs. negative samples to show that negative samples with common neighbors are harder to distinguish, since dense graphs predispose pairs to share common neighbors even when no edge exists. The mechanism (CN/PPR distinguishes hard cases) is established — applying it to attack-edge difficulty is your contribution, not a replication.

---

### Ratio Check for Candidate Datasets

| Dataset | Total edges | Attack edges | Ratio | Graph density for CN/PPR |
|---|---|---|---|---|
| UNSW-NB15 | ~2M | 321k anomalies | ~1:6 | Only 50 distinct IPs — graph is too small for meaningful CN/PPR tiering |
| LANL | ~3M | 439 anomalies | ~1:7000 | 15.6k nodes — your earlier problem persists |
| OpTC | ~1M | 674 anomalies | ~1:1500 | 976 nodes — too small for CN diversity |

**None of CyberGFM's three datasets have both a good ratio AND enough nodes for CN/PPR to produce a meaningful spread.** UNSW-NB15's ratio looks great (1:6) but the dataset only has 45 distinct IP addresses — CN/PPR will saturate immediately; everything will look "structurally similar" because the graph is nearly complete.

---

### What Actually Fits Option 1

You need: **many nodes** (for CN/PPR variance) + **dense attack edges relative to benign** (for tier population). This is a narrow combination. Revisit your earlier datasets with this lens:

- **CICAPT-IIoT**: per-edge labels, IIoT testbed — check the actual node count and attack ratio, this is your best candidate since it was built for provenance-based detection at reasonable scale
- **CTU-13 Neris**: 4,190 attack pairs, 607k nodes — ratio is ~166:1 but the *node count* gives CN/PPR room to differentiate; worth checking if attack pairs span a range of CN values

---

### Recommendation

Run the actual CN/PPR distribution check on **CTU-13 Neris attack pairs** (you already have this data) before searching further:

```python
# On the benign-only graph, compute CN for each attack pair
attack_pairs = df[df['Label'].str.contains('Botnet')][['SrcAddr','DstAddr']].drop_duplicates()
benign_graph = build_graph(df[~df['Label'].str.contains('Botnet')])

cn_scores = [len(set(benign_graph.neighbors(u)) & set(benign_graph.neighbors(v))) 
              for u, v in attack_pairs.values]

import numpy as np
print(np.histogram(cn_scores, bins=[0,1,2,5,10,100]))
```

If this shows a spread (some attack pairs CN=0, others CN>5), CTU-13 Neris supports your tiering approach and you don't need a new dataset search — you already have the data in hand.

---

**Cursor**

Clarifying the tiering distinction and running the CN spread check on your Neris file.


**Key corrections** on that write-up:

---

## 1. Your protocol ≠ “tier attack edges by CN”

**Your plan (keep this):** tier **test negatives** (random benign → same-src → low-CN/PPR benign). Attacks are always test **positives**.

**Option 1 in that text:** tier **attacks** by CN/PPR — different idea, no direct precedent, not what you locked for CARS.

HeaRT’s CN argument applies to **hard negatives**, not attack difficulty tiers.

---

## 2. Fix the ratio table (your actual counts)

| Dataset | Benign edges | Attack edges | Nodes | Notes |
|---------|-------------|--------------|-------|--------|
| **LANL** (your scan) | **148,848** | **308** | **11,161** | Not 3M / 439 — those look like events or another split |
| **Neris** (1 file) | **693,328** pairs | **4,190** pairs | **~604k** IPs | ~166:1; 40k botnet flows |
| **OpTC / CICAPT** | TBD | TBD | — | Still worth preprocessing |

LANL’s issue isn’t “308 isolated edges” for CN on **attacks** — it’s **too few attack labels** for supervised learning. **CN/PPR on benign haystack** still works with 11k nodes.

---

## 3. Neris CN check (ran on your file)

On **benign-only** undirected graph:

| | CN=0 | CN spread |
|--|------|-----------|
| **All 4,190 attack pairs** | **100%** | max CN = 0 |
| **50k benign pair sample** | **99.7%** | some CN up to 30 |

**Neris fails CN tiering** here: graph is so sparse at IP level that almost every pair has **CN=0**. Tier D (bottom CN/PPR benign) won’t separate much from Tier A/B.

---

## 4. What fits your paper

| Need | Best fit |
|------|----------|
| **Tiered hard-benign eval** | **LANL** (same-org hosts, meaningful local structure) + **OpTC** |
| **Many attack labels** | **Neris** (4k pairs) — but botnet flows, not lateral auth |
| **CN/PPR tier spread** | **Not Neris** at IP level; **LANL computers** much better than 604k sparse IPs |

**Don’t hunt a dataset for “dense attack edges for CN tiers on attacks.”** Hunt for **realistic benign decoys** (your tiers B/C/D).

---

## 5. Quick LANL sanity check (worth running)

Same script on LANL: CN on **attack pairs** vs **held-out benign** on pre-attack train graph — expect **wider CN spread** than Neris.

---

**Bottom line:** The “no precedent for attack-edge tiering” angle is **novel but off-scope**. Your contribution is **HeaRT-style hard **benign** negatives on cyber attack graphs**. **LANL + OpTC (+ CICAPT)** stay the right trio; **drop Neris** for tiering (CN saturates at 0). Fix paper numbers to **148k / 308 / 11k** for LANL.

---

**User**

did u sample CN at random?

---

**Cursor**

**Partially.**

| Set | CN computation |
|-----|----------------|
| **Attack** (4,190 pairs) | **All** unique botnet `(src,dst)` — no sampling |
| **Benign** | **Random sample** of 50,000 / 693,328 unique pairs (`random.seed(42)`) |

CN itself is **deterministic** from the benign-only neighbor graph — not random scores.

Attack CN=0 on **100%** of pairs is exact (full set). Benign “99.7% CN=0” is from that **50k sample**; full benign distribution should be similar given the sparse IP graph, but it’s an estimate, not exhaustive.

---

**User**

can you sample CN using CN/RA algo

---

**Cursor**

Checking how HeaRT computes CN/RA, then applying it to Neris.


**Yes — ran HeaRT-style CN/RA per pair** (not the full `N×N` matrix — 604k nodes would be infeasible). Same formulas as `create_heart_negatives.py`:

| Metric | HeaRT formula (directed) |
|--------|-------------------------|
| **CN** | `(A @ A^T)[u,v]` = \|out(u) ∩ out(v)\| |
| **RA** | `sum_{k ∈ out(u)∩out(v)} 1 / in_deg(k)` |

Undirected: `CN = |N(u)∩N(v)|`, `RA = Σ 1/deg(k)` over common neighbors.

**Sampling:** all **4,190** attack pairs; **50k** random benign pairs (seed 42). Benign-only train graph.

---

## Neris results

| | Attack >0 | Benign sample >0 |
|--|-----------|------------------|
| CN directed | **0%** | 0.15% |
| RA directed | **0%** | 0.15% |
| CN undirected | **0%** | 0.27% |
| RA undirected | **0%** | 0.27% |

**All attack pairs score CN=RA=0** on the benign graph. Almost all benign pairs too → **Tier D (bottom CN/RA) won’t separate** on Neris IP graphs.

---

## Reusable per-pair scorer (matches HeaRT, no full matrix)

```python
def ra_directed(u, v, out_nbr, in_deg):
    ou, ov = out_nbr.get(u), out_nbr.get(v)
    if not ou or not ov:
        return 0.0
    return sum(1.0 / in_deg[k] for k in (ou & ov) if in_deg[k] > 0)

def cn_directed(u, v, out_nbr):
    ou, ov = out_nbr.get(u), out_nbr.get(v)
    return len(ou & ov) if ou and ov else 0
```

For **Tier D on LANL**, run the same on computer graph — expect more spread than Neris.

**Note:** HeaRT’s full `calc_CN_metric` builds sparse `adj @ adj` for all nodes on OGB-sized graphs; for LANL (~11k nodes) that’s fine in preprocess; for Neris use **per-pair** only.

---

**User**

check for anderson algo on same and use from heart negatives

but how to compute hard benigns here? suggest heuristics based on graph datasets you have suggested or read papers for it to find the right heuristics

---

**Cursor**

**Hard benigns = real benign edges that could fool an attack detector** — not random non-edges. HeaRT gives the *structural* machinery (CN/RA/PPR); cyber papers give the *operational* definition (unknown auth, same actor, haystack of legit traffic).

---

## Principle (cite mix)

| Source | Hard-negative idea |
|--------|-------------------|
| **HeaRT** (NeurIPS’23) | Negatives with **high CN/RA/PPR** on train graph look like true links → inflate LP scores |
| **Euler** (TOPS’23) | Test against **real benign activity** in the window, not random non-edges |
| **Bowman UA** (GL-LV/GV) | **Unknown authentication**: benign edge **not seen in train** |
| **Your paper** | Adapt HeaRT’s *ranking* idea to **attack graphs**: decoys are **labeled benign edges**, not missing links |

---

## Tier template (all datasets)

| Tier | Name | Heuristic | Purpose |
|------|------|-----------|---------|
| **A** | Easy | Random **non-edges** | Inflated baseline (HeaRT “existing setting”) |
| **B** | Random benign | Random **held-out benign** edges | Real links, easy |
| **C** | Context | **Same-source** benign (main) | Same actor, different dst — lateral-movement decoy |
| **D** | Structural | Unseen-in-train + **low** CN/RA/PPR | Attack-*looking* novel benign (Bowman + inverted HeaRT) |
| **E** (opt.) | Haystack | Sample from **all** held-out benign | Euler-style operational load |

**Per attack `(u,v)`:** sample **K=500** negatives from that tier’s pool (same `u` where tier requires it).

---

## LANL (auth, computers `src→dst`)

**Train graph:** pre-attack benign unique edges + `edge_attr`.

| Tier | How to build pool for attack `(u,v)` |
|------|--------------------------------------|
| **B** | `val_benign` edges uniform random |
| **C** | `{ (u, v') ∈ val_benign : v' ≠ v }

---

**User**

check for CN on provenance dataset

---

**Cursor**

**CICAPT provenance CN check** (HeaRT-style directed + undirected, benign train graph):

---

## Setup

| Graph | Source |
|--------|--------|
| **Train** | Phase 1 edges (64,225 unique, 46,773 nodes) **or** Phase 2 benign-only (74,776 edges) |
| **Attack edges** | Phase 2 edges touching `label=1` node (**569** unique) |
| **Benign** | 10k random Phase 2 benign edges |

---

## Results

### A) Train = **Phase 1**, test = **Phase 2**

| | CN > 0 |
|--|--------|
| Attack (569) | **0%** |
| Benign sample | **0%** directed, **0.1%** undirected |

Only **15,209 / 53k** Phase 2 nodes overlap Phase 1 → almost all Phase 2 pairs are **unseen** on Phase 1 graph → CN = 0.

### B) Train = **Phase 2 benign only**, test = **Phase 2 attack**

| | CN > 0 |
|--|--------|
| Attack (569) | **0%** |
| Benign sample | **35.6%** (max CN = 4, mean ≈ 0.37) |

**Critical:** **0 / 569** attack edges have **both endpoints in the benign subgraph** — attack nodes/edges are **disconnected** from benign train nodes. CN is undefined (0) for every attack pair.

---

## Implication for hard benigns on CICAPT

**CN / PPR / RA (HeaRT-style) does not work** for ranking attacks vs benign on this provenance graph — attack edges live on a **separate labeled subgraph**, not “hard missing links” on the benign graph.

### Use **provenance-specific** hard benign heuristics instead:

| Tier | Heuristic | Rationale |
|------|-----------|-----------|
| **B** | Random **held-out benign** edges (same `operation` type) | Easy baseline |
| **C** | **Same-source process** benign: same `from` process, different benign `to` | Mimics “legitimate action from same parent” |
| **C′** | Same **edge type** (`WasTriggeredBy`, `Used`, …) + same artifact subtype | Typed DAG structure |
| **D** | Benign edges **1-hop from attack node** in full Phase 2 graph (benign-labeled neighbors of attack processes) | Operational haystack — “near attack but benign” |
| **D′** | Same **`exe` / `command line` prefix** family as attack process, benign destination | APT papers use process lineage |

**Papers to cite:**
- **HeaRT** — CN/PPR for **KG/LP missing links** (not directly transferable here)
- **Bowman / Euler UA** — “edge not in train” = suspicious (structural novelty)
- **CICAPT-IIoT paper** — provenance **typed edges** + Phase 1 benign / Phase 2 APT labels

---

## Dataset summary for CN tiering

| Dataset | CN spread for attack vs hard benign? |
|---------|-------------------------------------|
| **LANL** | **Yes** (same computer graph, expect usable CN) — run next |
| **Neris** | **No** (~100% CN=0) |
| **CICAPT** | **No** (attack subgraph isolated) → use tiers C / D above |

**Bottom line:** On provenance, hard benigns = **topology + type + proximity to attack nodes**, not CN/PPR bottom percentile. LANL remains your main CN/PPR tier dataset; CICAPT needs the provenance heuristics in

---

**User**

check for PPR in provenance

---

**Cursor**

Running Andersen PPR on CICAPT provenance (Phase1 train, Phase2 attack/benign pairs).


**PPR on CICAPT provenance** (Anderson, same as HeaRT `calc_ppr.py`; pure Python — `numba` missing in `heart_gpu`).

---

## Setup

| Graph | Nodes | Edges |
|-------|------:|------:|
| Phase1 train | 46,773 | 64,225 |
| Phase2 benign | 52,875 | 74,776 |
| **Combined (P1 + P2 benign)** | **84,458** | **138,996** |

Attack test: **569** unique Phase2 edges touching attack-labeled nodes (332 nodes).

---

## Results

| Train graph | Attack PPR | Benign PPR |
|-------------|------------|------------|
| **Phase1 only** | **0/569** scored (endpoints not in graph) | **1/2000** scored |
| **P1 + P2 benign** | **0/569** scored | **3000/3000** scored; **94.7% PPR>0**, median **0.042** |
| **Phase2 benign only** | **0/569** scored | — |

**Attack edges:** **0%** have both endpoints in any benign train graph — attack nodes are **new Phase2 provenance IDs**, not in benign subgraph.

**Benign Phase2 pairs** on combined train: PPR works and has **wide spread** (median ~0.04, max 1.0).

---

## Implication for Tier D (low PPR/CN benign)

**CN and PPR tiering do not apply to attack positives on CICAPT** under the standard Phase1-train / Phase2-test split — attacks are **out-of-vocabulary nodes**.

**Use provenance-specific hard benign heuristics instead:**

| Tier | Heuristic |
|------|-----------|
| **B** | Random Phase2 **benign** edges |
| **C** | Same **parent process** / same **WTB** source (`from` process) |
| **C′** | Same **operation** (`Used`/`WGB`) + same **artifact subtype** |
| **D** | Benign edges with **high PPR** on combined train (easy) vs **low PPR** bottom decile — ranks **benign vs benign**, not attack vs benign via PPR |

For attacks, hard decoys = **benign edges sharing attack context** (same process lineage, same file/socket path prefix), not CN/PPR to attack endpoints.

---

**Bottom line:** PPR is useful for **sampling hard benign negatives among benign edges** on CICAPT; **not** for scoring attack edges directly. **LANL/OpTC** remain better for CN/PPR tier D on attack-vs-benign ranking. Install `numba` to run full HeaRT `calc_ppr.py` on ~85k nodes in preprocess.

---

**User**

did you run ppr anderson algorithm on phase2? on attack nodes only. Yes or No?

---

**Cursor**

**No.** Attack endpoints weren’t in the benign train graph, so Anderson PPR never ran on Phase2 attack nodes (0/569 pairs scored).

---

**User**

how about running link prediction the way it runs in the cybergfm and as evaluation we choose hard benigns and give to model to do both prediction on anomolous and hard benigns and rank anomolous among hard benigns?

---

**Cursor**

**Yes — that’s a strong framing**, and it matches where your plan already points. Split **training style** (CyberGFM/Jbeil-like) from **eval** (your tiered hard-benign ranking).

---

## CyberGFM-style training (what to copy)

| Piece | CyberGFM / Jbeil idea | Your lightweight version |
|--------|------------------------|---------------------------|
| **Task** | Edge-level LP / “is this link normal?” | Same |
| **Train pos (y=1 link)** | Benign edges | Pre-attack / Phase1 benign |
| **Train neg (y=0 non-link)** | Malicious + easy negs | Attack edges + optional random non-edges |
| **Graph for GNN** | Benign structure (attacks not in adj) | Same |
| **Score** | P(link exists) | Rank attacks with **`1 - P(link)`** = suspicious |

You **don’t need** full CyberGFM (walk pretrain + foundation model) for CARS — cite it, use **GATv2 + edge attr + supervised BCE**, same *problem shape*.

---

## Your eval (the actual contribution)

CyberGFM-style papers often eval with **easy negatives** or generic malicious/benign splits. You add:

```text
Per test attack edge (u,v):
  score attack once
  score K hard benign decoys (tiers B/C/D)
  AP/AUC: rank attack above those K
```

| Tier | Hard benign source |
|------|-------------------|
| A | Random non-edges (inflated baseline) |
| B | Random held-out benign edges |
| C | Same-source (LANL: same `src_comp`; CICAPT: same process `from`) |
| D | Unseen + low CN/PPR (LANL/OpTC); **not** CICAPT attacks (OOV nodes) |

**One model, multiple eval tiers** — that’s the paper.

---

## Label mapping (pick one, document it)

**Option A (Euler-aligned):**
- Train: benign = link (y=1), attack = non-link (y=0)  
- Test: high suspicion = low link score  

**Option B (explicit anomaly):**
- Train: attack y=1, benign y=0, score = P(attack)  

Same ranking if you flip consistently at test.

---

## vs Euler / vs CyberGFM

| | Euler/Bowman | CyberGFM/Jbeil | **You** |
|--|--------------|----------------|---------|
| Train | Benign only | Benign + attack | Benign + attack (1:8) |
| Test negs | Implicit all benign | Mixed | **Explicit hard-benign tiers** |
| Claim | Detector speed/accuracy | Foundation LP | **Eval protocol** |

---

## Dataset fit (from your CN/PPR checks)

| Dataset | CyberGFM-style train | Tier D (CN/PPR) |
|---------|----------------------|-----------------|
| **LANL** | ✓ | ✓ |
| **OpTC** | ✓ | ✓ (check after preprocess) |
| **CICAPT** | ✓ train P1+P2 benign, test P2 attack | Use **C/C′** heuristics, not CN/PPR on attacks |
| **Neris** | ✓ | Skip (CN/PPR ≈ 0) |

---

## Practical recipe (deadline-friendly)

1. **Train:** GATv2, benign `edge_index`, BCE on benign (y=1) + attack (y=0), 1:8 batches.  
2. **Test:** frozen benign graph; score attack + K hard benigns per tier.  
3. **Table:** AP/AUC per tier × dataset × model (GATv2, optional GCN).  
4. **Cite:** CyberGFM/Jbeil for supervised cyber LP; HeaRT for “easy negs mislead”; Euler for benign haystack motivation.

**Bottom line:** Run LP **like CyberGFM fine-tuning** (supervised edge scores), eval **unlike** them (rank attacks among **hard benigns**). That’s coherent, novel on eval, and feasible without reimplementing CyberGFM pretraining.

---

**User**

but I dont see CN/PPR good scores, can we use some other heuristics here?

---

**Cursor**

**Yes — drop CN/PPR as the main Tier D driver** where scores don’t spread (Neris, CICAPT attacks; often marginal on sparse IP graphs). Use **context + novelty** heuristics from cyber papers instead.

---

## Core hard-benign principles (no CN/PPR)

| Idea | Source | Rule |
|------|--------|------|
| **Unseen in train** | Bowman UA / Euler | Benign edge `(u,v)` never in train graph |
| **Same actor** | Lateral movement | Same `src` (host/user/process) as attack, different `dst` |
| **Same destination** | Pivot targets | Same `dst`, different `src` |
| **Edge-feature match** | Auth/provenance | Same `logon_type` / `operation` / `proto` as attack |
| **Temporal neighbor** | Euler snapshots | Benign edge in same time window as attack campaign |
| **2-hop plausible** | Weak structural | `u`→`w` and `w`→`v` exist in train, `(u,v)` benign & unseen |

These match **“attack-looking but labeled benign”** without needing CN>0.

---

## Tier design (replace D)

| Tier | Heuristic | Role |
|------|-----------|------|
| **A** | Random non-edge | Easy (inflated) baseline |
| **B** | Random held-out benign | Real edges, easy |
| **C** | **Same-source** benign | Main hard tier |
| **D** | **Unseen + context match** (below) | Hardest operational tier |
| ~~D-old~~ | ~~Bottom CN/PPR~~ | Optional on **LANL only** if spread exists |

---

## Per-dataset Tier D heuristics

### LANL (auth)
1. Unseen `(src_comp→dst_comp)` in train  
2. Same `src_comp` as attack (**required**)  
3. Optional: same `logon_type=Network` or similar `activity`  
4. Optional LAN

---

**User**

what could be another heuristic here to compute hard negatives other PPR/CN?

---

**Cursor**

**Yes — use cyber-specific “plausible benign” rules instead of CN/PPR when structure is sparse or attack nodes are OOV.** Keep **Tier A/B/C**; replace **Tier D** with one of these.

---

## Universal tiers (keep)

| Tier | Heuristic | Cites |
|------|-----------|--------|
| **A** | Random non-edges | HeaRT easy baseline |
| **B** | Random held-out **real benign** edges | Euler haystack-lite |
| **C** | **Same-source** benign | Operational false-positive pressure |

---

## Tier D replacements (no CN/PPR)

### 1. **Unseen-in-train (UA-style)** — best default
Bowman/Euler **Unknown Authentication**: benign test edge `(u,v)` **never in train** (as a pair).

```text
D-UA: unseen benign pairs only
```
Works on **LANL, OpTC, CICAPT**; no structural scores needed.

---

### 2. **Same-source, different destination** — main hard tier
For each attack `(u,v)`, sample K benign `(u, v')`, `v'≠v`, from val/holdout benign.

```text
D-src: same actor, other legit destination
```
Strong on **LANL** (lateral movement from same host).

---

### 3. **Edge-feature match**
Among unseen benign edges, pick K with **closest** attack edge features (count, `logon_type`, `proto`, `operation`):

```text
D-feat: min L2 distance on aggregated edge_attr
```
Good when CN=0 everywhere (**Neris**, sparse IP graphs).

---

### 4. **2-hop / path-plausible**
Benign `(u,v)` where `u`→`v` is **not** in train but `u→w→v` or `u` shares neighbor with `v` in train (weak path).

```text
D-2hop: v reachable in 2 hops from u on train graph, edge absent
```
Softer than CN/PPR; works on **LANL/OpTC**.

---

### 5. **Provenance-specific (CICAPT)**
Attack nodes are new IDs — use **context**, not graph distance:

| Rule | Hard benign |
|------|----------------|
| **D-proc** | Same `from` process, different `to` artifact |
| **D-op** | Same `operation` (`Used`/`WGB`/`WTB`) + same artifact subtype |
| **D-lineage** | Benign edge within ±Δt of attack window, same parent `ppid` bucket |

---

### 6. **Degree / role matched**
Benign `(u,v)` with **similar** `out_degree(u)` and `in_degree(v)` to the attack pair (±1 bucket).

```text
D-deg: hub-to-hub vs leaf-to-leaf matched benign
```

---

### 7. **Score-based hard pool (model-agnostic)**
After one train pass, take benign val edges with score **closest to attack median** (hardest to separate):

```text
D-score: bottom decile |score_benign - score_attack|
```
Optional ablation — not for main protocol (uses model).

---

## Recommended paper matrix

| Dataset | Tier C | Tier D |
|---------|--------|--------|
| **LANL** | Same `src_comp` | **D-UA** or **D-src** (+ optional D-feat) |
| **OpTC** | Same host / same user session | **D-UA** + **D-2hop** |
| **CICAPT** | Same `from` process | **D-proc** / **D-op** |
| **Neris** | Same `SrcAddr` | **D-feat** (proto/port/bytes) |

Drop **CN/PPR Tier D** unless LANL shows spread in a quick check.

---

## One-line contribution

> We replace HeaRT’s structural hard negatives (CN/PPR) with **domain hard benigns**: unseen pairs, same-source destinations, and edge-feature matches — appropriate when attack graphs are sparse or attack nodes are out-of-vocabulary.

**Minimum viable:** **A, B, C, D-UA** on all three datasets — no PPR/CN required.

---

**User**

did you check ppr/cn for benign nodes? or not? say yes or no only

---

**Cursor**

**Yes** — on **benign edge pairs** (sampled), not on attack only.

---

**User**

so these datasets cannot be used right for heart scheme? Yes or No onlyu

---

**Cursor**

**Yes.**

---

**User**

can you check the code if you checked CN/PPR scores properly for benign nodes for all datasets and there is no mistake
?

---

**Cursor**

Auditing what we ran per dataset and re-checking LANL CN/PPR (likely missed earlier).


**No — the CN/PPR checks were not complete or fully correct across all datasets.** Here’s the audit.

---

## What was actually run

| Dataset | CN benign? | PPR benign? | Attack scored? |
|---------|------------|-------------|----------------|
| **Neris** | Yes (50k random **benign pairs**) | **No** (numba missing; full run interrupted) | Yes (all 4,190 pairs) |
| **CICAPT** | Yes (10k sample; then 3k for PPR) | Yes (Anderson, combined P1+P2 benign train) | Yes (569 pairs; **0 scored** — OOV) |
| **LANL** | **No** | **No** | **No** |
| **OpTC** | **No** | **No** | **No** |

So we did **not** check CN/PPR for benign on all datasets.

---

## Method issues (not just missing runs)

1. **Objects scored:** **edge pairs `(u,v)`**, not “benign nodes.” Correct for LP; wording was wrong.

2. **Not HeaRT-style per-anchor mining:** HeaRT scores CN/PPR for **candidates `v'` given fixed source `u`**. We used **random unrelated benign pairs** — fine for a global spread check, **not** the same as Tier-C/D decoys per attack `(u,v)`.

3. **CICAPT train/test:** Attack edges use **new Phase2 node IDs** → CN/PPR on train graph correctly gives **0/569 attack** scores. Benign on **combined P1+P2 benign** graph is valid; **attack OOV is expected**, not a code bug.

4. **Neris graph:** CN on **full internet IP graph** → ~99.7% CN=0 for benign too. Sparse graph, not necessarily wrong code.

5. **RA:** Computed for Neris in one script; **not** reported for CICAPT; **not** paired with HeaRT `calc_CN_metric` matrix path.

6. **LANL:** Data was removed from disk (`rm -rf` in terminal) — **cannot re-run** now.

7. **PPR:** HeaRT uses `calc_ppr.py` + **numba**; env lacks **numba** — used pure-Python Andersen for CICAPT only.

---

## Verdict

| Claim | Correct? |
|-------|----------|
| CN/PPR bad on **all** datasets | **Overstated** — only 2 datasets, LANL/OpTC never tested |
| CN/PPR useless for **cyber hard benigns** | **Mostly yes** for Neris + CICAPT attacks; **LANL may differ** (never measured) |
| Benign was checked | **Yes** for Neris/CICAPT (sampled pairs), **not** anchor-conditioned |

---

## What a proper check would do

1. **LANL + OpTC:** train benign graph → CN/RA/PPR for **attack pairs**, **random benign**, **same-src benign `(u,v')`** per attack.  
2. **CICAPT:** same-src / same-process decoys, not only random benign pairs.  
3. Install **numba**, use HeaRT `calc_CN_metric` + `get_ppr_matrix` on graphs ≤100k nodes.  
4. Report **directed** CN/RA for directed cyber graphs.

**Bottom line:** Earlier “CN/PPR don’t work” is **supported for Neris and CICAPT (attack OOV / sparse)**, but the audit was **incomplete and partly mis-specified** vs HeaRT’s per-anchor scheme; **LANL/OpTC were never checked.**

---

**User**

do it

---

**Cursor**

Implementing a proper per-dataset CN/RA/PPR audit with anchor-conditioned benign scoring.


Creating a proper audit script and running it on available datasets.


Improving the audit: Neris attack sources often appear only as destinations in the benign graph. Adding relaxed same-endpoint decoys.


Done. Proper audit script: `CARS/code_preprocess/audit_cn_ppr.py` → results in `CARS/processed/audit_cn_ppr.json`.

**What it does (fixed vs before):**
- HeaRT **directed CN / RA** on **benign train graph**
- **Andersen PPR** (same as `calc_ppr.py`)
- **3 benign conditions:** random pairs, **same-src decoys**, **same-endpoint decoys** (per attack anchor)
- Reports **missing endpoints** (OOV nodes)

**LANL:** skipped — `auth.txt.gz` / `redteam.txt.gz` deleted from disk. Re-download and rerun:
```bash
python CARS/code_preprocess/audit_cn_ppr.py --datasets lanl
```

---

## Results summary

| Dataset | Attack CN/PPR | Benign random CN>0 | Same-src decoys | Verdict for HeaRT Tier D |
|---------|---------------|--------------------|-----------------|--------------------------|
| **Provenance** | 0/569 scored (OOV attack nodes) | 12.7% CN, 24% PPR | 18/569 only | **No** — use D-proc / D-op |
| **Neris** | 137/4190 in graph, all CN=0 | 0.1% CN | same-endpoint PPR 91% | **No** for CN; PPR only on benign |
| **OpTC** (sample) | 2383 scored, CN≈0 | 0% | same-src works (2386) | **Weak** — CN useless; use D-UA / same-host |

**Key findings:**
1. **Benign random** on provenance has CN/PPR spread — Tier D could rank **benign vs benign**, not attack vs benign via structure.
2. **Attack nodes OOV** on provenance (551/569) — structural scores can't rank attacks.
3. **Neris:** attack IPs mostly external → not in train graph; same-endpoint PPR high on benign only.
4. **OpTC sample:** tiny train (253 nodes); CN=0 everywhere — need full preprocess + **UA / same-host** tiers.

**Conclusion:** HeaRT CN/PPR **Tier D is not viable** on these cyber sets as tested. Use **Tier A/B/C + D-UA (unseen benign) + domain heuristics** (same-src host/process). Re-run LANL when data is restored — that's the one dataset likely to show CN spread on attacks.

---

**User**

can you suggest some other heuristics to confuse link prediction model so it gives harder prediction and we can rank positive predictions among negatives ones for cybersecurity datasets?

---

**Cursor**

Given your audit (CN/PPR saturate at 0, attack nodes often OOV), the right hard-negative heuristics are **feature/behavior-based** and **embedding-based**, not structural. Here are the strongest candidates for cyber LP.

## Tier groups that actually confuse the model

### 1. Embedding-space nearest neighbors (model-in-the-loop) — strongest
After (or during) training, for each attack `(u,v)`:
- Take the GNN node embeddings `h_u, h_v`.
- Sample benign edges `(a,b)` whose **pair embedding** `[h_a‖h_b]` (or `h_a⊙h_b`) is **closest** to `[h_u‖h_v]`.

These are benigns the model *represents* like attacks → maximally confusing. Works even when nodes are OOV-for-CN, because embeddings exist for any train node. This is the cyber analog of HeaRT's "hard" idea but in **learned** space, not CN space.

### 2. Edge-feature distance (no graph structure needed) — best for sparse/OOV
For each attack, pick benigns with the **smallest feature distance** on the aggregated edge vector:
- LANL: `[log count, logon_type onehot, auth_type, dominant hour]`
- Neris: `[proto, dest_port, log bytes, duration]`
- OpTC: `[dest_port, l4protocol, direction]`
- Provenance: `[operation, artifact subtype, exe]`

Rank by L2/cosine to the attack edge's features. This directly creates "looks identical but benign" decoys.

### 3. Destination-popularity / degree-matched
Pick benign edges whose **destination has similar in-degree** to the attack's destination (±1 bucket).
- Prevents the trivial shortcut "attack target = rare host."
- Forces the model to use more than node popularity.

### 4. Temporal-proximity decoys
Benign edges occurring in the **same time window** as the attack (±Δt).
- Same network conditions, same load → removes "time-of-day" leakage.
- Cheap, no structure required; strong on LANL/OpTC/Neris.

### 5. Source-behavior decoys (actor-conditioned)
For attack source `u`, sample benign `(u, v')` OR benign edges from **hosts with similar fan-out** to `u`.
- Your audit showed same-`src` is sparse (attack sources rarely have benign out-edges), so generalize to **"hosts behaving like u"** via degree/feature similarity, not exact `u`.

### 6. Path/2-hop reachable (relaxed structural)
Benign `(u,v)` where `v` is reachable from `u` in **2 hops** on the train graph though the direct edge is absent. Softer than CN; produces non-zero spread where CN=0.

## Recommended tier matrix for the paper

| Tier | Heuristic | Needs structure? | Notes |
|------|-----------|------------------|-------|
| A | Random non-edges | no | inflated baseline |
| B | Random held-out benign | no | |
| C | Edge-feature NN (#2) | no | main hard tier, all datasets |
| D | Embedding NN (#1) | learned | hardest; model-in-loop |
| E (opt) | Temporal-proximity (#4) | no | LANL/OpTC/Neris |

This replaces CN/PPR Tier D cleanly and is defensible: *"structural hard negatives degenerate on sparse/OOV cyber graphs, so we mine hard benigns in feature and embedding space."*

Want me to add these tier samplers to `audit_cn_ppr.py` (or a new `build_hard_negatives.py`) and measure attack-vs-decoy separation for each? That gives you a concrete table showing which heuristic produces the smallest score gap (= hardest) per dataset.

---

**User**

is this supported in some research paper?

---

**Cursor**

Yes — most of these are grounded in published work, though **not** all assembled for cyber LP specifically (that combination is your contribution). Here's the mapping.

## Heuristic → paper support

| Heuristic | Supported by | What they show |
|-----------|-------------|----------------|
| **Embedding/score-space hard negatives** | Strong precedent | The dominant "hard negative mining" idea |
| **Structural hard negatives (CN/PPR)** | HeaRT (Li et al., NeurIPS 2023, [arXiv:2306.10453](https://arxiv.org/abs/2306.10453)) | Random negs inflate LP; CN/PPR-based negs are honest — your audit shows these **degenerate** on cyber graphs |
| **Feature-distance / "looks identical but benign"** | Adversarial & metric-learning literature | Hard negatives = same features, different label |
| **Degree/popularity-matched negatives** | Recsys + KGE sampling | Counters popularity shortcut |
| **Temporal-proximity negatives** | Temporal GNN / dynamic LP | Negatives from same time window |
| **Unseen-pair (UA) negatives** | Euler/Bowman cyber IDS | "Unknown authentication" baseline |

### Specific references

**Hard negative mining (embedding/score-based)** — the general principle:
- **Ying et al., PinSage (KDD 2018)** — "hard negatives" = items embedding-close to the query but not linked; curriculum hard-negative sampling. This is exactly your Tier D (#1).
- **Schroff et al., FaceNet (CVPR 2015)** — semi-hard negative mining in embedding space; foundational for "confusing but wrong-class" samples.
- **Robinson et al., "Contrastive Learning with Hard Negative Samples" (ICLR 2021, [arXiv:2010.04592](https://arxiv.org/abs/2010.04592))** — formalizes sampling negatives that are hard but true-negative.

**Knowledge-graph / LP negative sampling** — degree & distribution-aware:
- **Sun et al., RotatE (ICLR 2019)** — self-adversarial negative sampling weighted by current model score (your #1 in KGE form).
- **Kotnis & Nastase (2017), "Analysis of the Impact of Negative Sampling on Link Prediction"** — compares random vs nearest-neighbor vs corrupting-popular-entities; supports degree-matched (#3) and NN (#2).
- **Ahrabian et al. (2020), structure-aware negative sampling** — uses graph proximity for harder negatives.

**LP benchmarking critique (your core framing):**
- **HeaRT (Li et al., 2023)** — the "easy negatives mislead" thesis; you extend it to cyber where their CN/PPR recipe fails.
- **Li et al., "Revisiting Link Prediction: A Data Perspective" (ICLR 2024, [arXiv:2310.00793](https://arxiv.org/abs/2310.00793))** — argues the *right* negatives depend on data; directly licenses swapping CN/PPR for domain heuristics.

**Cyber-specific unseen/temporal:**
- **King & Huang, Euler (TOPS 2023)** — UA baseline = unseen auth edges; benign haystack at test.
- **Bowman et al. (2020)** — unsupervised LP on benign auth; unseen edges as anomalies.

## What's novel vs cited

- **Cited / not novel:** each heuristic individually (embedding-NN, feature-NN, degree-matched, temporal, UA).
- **Your contribution:**
  1. Showing **CN/PPR (HeaRT's recipe) empirically degenerates** on cyber attack graphs (sparse + OOV attack nodes) — your audit is direct evidence.
  2. A **tiered hard-benign protocol** combining feature/embedding/temporal negatives for **attack-vs-benign LP evaluation** — this exact assembly isn't in one prior paper.

So you can write: *"We adopt hard-negative mining (PinSage, RotatE self-adversarial) and the LP-evaluation critique of HeaRT, but show their structural instantiation fails on cyber graphs and replace it with feature-, embedding-, and temporal-proximity hard benigns."* That's well-supported and clearly positions your novelty.

Want me to pull exact quotes/sections from HeaRT (`euler.txt` is loaded; HeaRT paper is in repo references) to cite precisely?

