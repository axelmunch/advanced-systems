FROM tomcat:jdk21

LABEL maintainers="amunch@cfai-formation.fr, wrakotozafy@cfai-formation.fr"

# Remplacer le fichier ROOT.war par le fichier jenkins.war pour accéder à l'URL http://localhost:8080
ADD https://get.jenkins.io/war-stable/2.479.3/jenkins.war /usr/local/tomcat/webapps/ROOT.war

# Si nous avons un fichier jenkins.war dans le répertoire courant (recommandé pour avoir le fichier à disposition sans avoir à télécharger)
# COPY jenkins.war /usr/local/tomcat/webapps/ROOT.war

EXPOSE 8080

CMD ["catalina.sh", "run"]