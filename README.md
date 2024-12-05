                          SaFeMed: A monitoring system for medications storage.
  
    This project addresses the Grand Challenge of eradicating public health issues by developing an intelligent monitoring system for medication storage. Improper storage conditions can compromise the efficiency of pharmaceuticals, leading to public health risks. Our proposed system continuously monitors three critical environmental variables: temperature, humidity, and light intensity, ensuring medication integrity. the system integrates advanced sensors with a real-time data analytics platform to detect deviations from optimal storage conditions, triggering timely alerts. Testing validated the system's ability to maintain compliance with pharmaceutical standards under varying environmental scenarios. The system design requirements are: first, the alerting system when readings pass the threshold. Second is the data-storing efficiency. The system must store one year's readings in less than 10 MB. The third design requirement is sustainability of the system; the system must live up to 3 years without any battery replacement. The prototype consists of two parts: a hard part of cooling storage and the multisensory system applied in it. This multisensory system consists of an ESP32 connected to a DHT11 sensor for measuring temperature and humidity and an LDR sensor measuring light intensity. All the sensor readings will be tabulated in Google Spreadsheets that will be later graphed and represented in the dashboard, ThingsBoard  platform. Readings in Google Spreadsheets, captured every 20 seconds, will be hourly averaged for storing them for later studies or analysis. After constructing the prototype, some tests must be held to test the system's validity. The system maintained a great alerting mechanism and storage of 1.2 MB per year, and lastly, the system achieved an average of 3.038 years life span without any battery replacement. After going through all of this, some key insights were found. Most of these findings pertain to the relationships between different datasets. The first relationship pertains to humidity and temperature, while the second examines the relationship between temperature and light intensity. Another significant observation is that the ESP32 is considerably less complex, more practical, and more energy efficient than other motherboards like Arduino. The system has achieved minimal complexity and remarkable storage, and cost-effectiveness compared to other prior solutions.
![image](https://github.com/user-attachments/assets/a565cefc-f58c-483f-98bd-d3e866099c73)
