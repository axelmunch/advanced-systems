# Docker basics

## 1. Run jenkins as a service

### Using `docker run` command

Pour installer jenkins en tant que Docker as a Service, lancez la commande suivante:

```shell
docker run --name winaxjenkins-stateless -p 8080:8080 jenkins/jenkins:lts-jdk17
```

Cependant, cette configuration n'est pas persistente, donc au redémarrage du conteneur, les données seront perdues. Ainsi, il est nécessaire de persister les données avec la commande suivante:

```shell
docker run --name winaxjenkins-stateful -p 8080:8080 jenkins/jenkins:lts-jdk17 -v jenkins_data:[temp]
```
### Using `docker compose`

## 2. Build jenkins service "from scratch"