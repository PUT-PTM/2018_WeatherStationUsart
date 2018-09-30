package com.weather;

import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;

import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.Date;

import java.io.FileDescriptor;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class Main {

    void connect(String portName) throws Exception {
        CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);
        if (portIdentifier.isCurrentlyOwned()) {
            System.out.println("Error: Port is currently in use");
        } else {
            CommPort commPort = portIdentifier.open(this.getClass().getName(), 2000);

            if (commPort instanceof SerialPort) {
                SerialPort serialPort = (SerialPort) commPort;
                serialPort.setSerialPortParams(115200, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);

                OutputStream out = serialPort.getOutputStream();

                (new Thread(new SerialWriter(out))).start();

            } else {
                System.out.println("Error: Only serial ports are handled by this example.");
            }
        }
    }

    public static class SerialWriter implements Runnable {
        OutputStream out;

        public SerialWriter(OutputStream out) {
            this.out = out;
        }

        public String getCity(String city) throws IOException {
            String s = "";

            String appid = "0aa634ac9a3758b22253d8765c81ebc2";
            String serverLink = "https://api.openweathermap.org/data/2.5/weather?q=" + city + "&units=metric&appid=" + appid;
            String charset = "UTF-8";

            URLConnection connection = new URL(serverLink).openConnection();
            connection.setRequestProperty("Accept-Charset", charset);
            InputStream response = connection.getInputStream();

            try (Scanner scanner = new Scanner(response)) {

                String responseBody = scanner.useDelimiter("\\A").next();

                JSONObject obj = new JSONObject(responseBody);
                JSONObject main = obj.getJSONObject("main");

                String name = obj.getString("name");
                String temp = "" + main.getInt("temp") + "C";
                String press = "" + main.getInt("pressure");
                String cloud = "" + obj.getJSONObject("clouds").getInt("all") + "%";

                Date sunDate = (new Date(obj.getJSONObject("sys").getLong("sunrise")));
                String sun = "" + sunDate.getHours() + ":" + sunDate.getMinutes();

                Date moonDate = (new Date(obj.getJSONObject("sys").getLong("sunset")));
                String moon = "" + moonDate.getHours() + ":" + moonDate.getMinutes();

                // Tutaj tworzyc zdanie
                s = name + "@" + temp + "@" + press + "@" + cloud + "@" + sun + "@" + moon;

            } catch (JSONException e) {
                e.printStackTrace();
            }

            return s;
        }


        public void run() {
            try {

                Scanner scanner = new Scanner(System.in);

                String city = "";

                do {
                    System.out.println("Podaj miasto: ");
                    city = scanner.next();

                    String s = getCity(city);

                    System.out.println(s);

                    for (int i = 0, n = s.length(); i < n; i++) {
                        char c = s.charAt(i);

                        this.out.write(c);
                    }

                    this.out.write('`');

                } while (!city.equals("end"));


            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        try {
            (new Main()).connect("COM5");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
