# Docker basics

## 1. Run jenkins as a service

### Using `docker run` command

Pour installer jenkins en tant que Docker as a Service, lancez la commande suivante en publiant le port 8080 pour accéder au service:

```shell
docker run --name jenkins-stateless -p 8080:8080 jenkins/jenkins:lts-jdk17
```

Le service sera accessible par l'adresse suivante: [http://localhost:8080](http://localhost:8080)

Cependant, cette configuration n'est pas persistente, donc au redémarrage du conteneur, les données seront perdues. Ainsi, il est nécessaire de persister les données avec la commande suivante:

```shell
docker run --name jenkins-statefull -p 8081:8080 -v jenkins_statefull:/var/jenkins_home jenkins/jenkins:lts-jdk17 
```

Cette seconde version du service sera accessible par l'adresse suivante: [http://localhost:8081](http://localhost:8081)

### Using `docker compose`

Pour installer Jenkins as a Service, nous pouvons opter par l'utilisation de `docker-compose.yaml`.

Pour cela, il suffit de lancer la commande suivante:

```bash
docker-compose up -d
```

Ce service livré avec Docker Compose sera accessible par l'adresse: [http://localhost:8082](http://localhost:8082)

## 2. Build jenkins service "from scratch"

Pour livrer le service Jenkins as a Service, nous pouvons build une image Docker en se basant sur l'image `tomcat`, le serveur web de Jenkins, comme base.

Les instructions de build sont présentes dans le fichier [Dockerfile](./Dockerfile).

Pour créer l'image, en vous positionnant dans le répertoire où est présent le fichier `Dockerfile` lancez la commande suivante:

```bash
docker build -t jenkins-scratch:1.0.0 .
```

Puis créez un conteneur avec l'image nouvellement créer en publiant un port disponible en persistant le volume:

```bash
docker run --name jenkins-scratch -p 8083:8080 -v jenkins_scratch:/var/jenkins_home jenkins-scratch:1.0.0 
```

Ce service from scratch de Jenkins sera accessible par l'adresse: [http://localhost:8083](http://localhost:8083)
