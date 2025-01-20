FROM tomcat:jdk21

ADD https://get.jenkins.io/war-stable/2.479.3/jenkins.war /usr/local/tomcat/webapps/

WORKDIR /usr/local/tomcat/webapps/

EXPOSE 8080

CMD ["java", "-jar", "jenkins.war"]
