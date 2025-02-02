FROM tomcat:jdk21

LABEL maintainers="amunch@cfai-formation.fr, wrakotozafy@cfai-formation.fr"

ADD https://get.jenkins.io/war-stable/2.479.3/jenkins.war /usr/local/tomcat/webapps

# Si nous avons un fichier jenkins.war dans le répertoire courant
# COPY jenkins.war /usr/local/tomcat/webapps/

# WORKDIR /usr/local/tomcat/webapps/

EXPOSE 8080

# CMD ["java", "-jar", "jenkins.war"]
CMD ["catalina.sh", "run"]