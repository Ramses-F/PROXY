# Proxy HTTP avec Cache et Interface d'Administration

## 🌟 Description

Ce projet est un **proxy HTTP multithread** développé en **C**. Il permet d'intercepter les requêtes HTTP GET, de les rediriger vers le serveur distant, et d'enregistrer les réponses dans un **cache local** pour accélérer les futures requêtes. Il comprend également une **interface Web d'administration** du cache.

## 📚 Objectifs Pédagogiques

* Comprendre le fonctionnement d'un serveur proxy HTTP.
* Implémenter un système de cache rudimentaire.
* Améliorer les performances des réseaux clients.
* Appliquer des notions de **sécurité** réseau via des tests de type pentest.

## 📊 Fonctionnalités

* ⚡️ Interception et redirection des requêtes HTTP GET.
* 🌎 Connexions client multiples grâce à `fork()`.
* 📁 Système de cache local avec `CACHE HIT` / `CACHE MISS`.
* 🌐 Interface Web pour visualiser ou supprimer le cache.
* 🔒 Tests de sécurité (buffer overflow, stress test via `ab`).

## 📆 Structure du projet

```
/proxy.c           # Serveur proxy principal
/cache.c           # Fonctions de gestion du cache
/cache.h           # Header du cache
/README.md         # Documentation du projet
```

## 🚀 Utilisation

### Lancer le proxy sur le port 8889

```bash
make
./proxy
```

### Faire une requête HTTP

```bash
curl -x localhost:8889 http://google.com
```

### Interface d'administration

Ouvrir un navigateur et aller sur `http://localhost:8889/admin` pour consulter ou purger le cache.

## 🤝 Exemples de commandes utiles

```bash
# Test de charge
ab -n 1000 -c 50 http://localhost:8889/example.com

# Test de débordement de tampon (pentest simple)
printf "GET /$(python3 -c 'print("A"*2000)') HTTP/1.1\r\nHost: site.com\r\n\r\n" | nc localhost 8889
```

## 🔧 Technologies

* **Langage :** C (ANSI)
* **Réseaux :** Sockets TCP
* **Tests :** netcat, Apache Benchmark (ab), curl

## 🛡️ Sécurité

* Protection basique contre les attaques de type buffer overflow.
* Journalisation des accès suspectes.
* Simulation de stress test pour identifier les limites du serveur.

## 📊 Améliorations possibles

* Utilisation de threads au lieu de fork pour une meilleure performance.
* Implémentation d'une politique de cache (FIFO, LRU).
* Chiffrement des connexions (HTTPS).
* Ajout d'une authentification à l'interface admin.

## 📄 Licence

Ce projet est réalisé dans un cadre **académique** et peut être utilisé à des fins pédagogiques.

---

> ⚡ Créé par Fallet Kamagaté dans le cadre d'un projet de Cybersécurité
