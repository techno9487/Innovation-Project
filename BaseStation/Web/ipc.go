package main

import (
	"encoding/json"
	"log"
	"net"
	"net/http"
	"strconv"
)

type DeviceMessage struct {
	DevceID int
}

type RawDeviceMessage struct {
	DeviceID int    `json:"device_id"`
	Content  string `json:"content"`
}

var ipcChan chan DeviceMessage

func initIPC() error {
	ipcChan = make(chan DeviceMessage, 10)

	ipcSocket, err := net.Dial("unix", "/tmp/project.sock")
	if err != nil {
		return err
	}

	go func(socket net.Conn) {
		for {
			msg := <-ipcChan

			contentRaw := map[string]string{}
			contentRaw["type"] = "action"

			data, err := json.Marshal(&contentRaw)
			if err != nil {
				log.Println(err)
				continue
			}

			rawMessage := RawDeviceMessage{
				DeviceID: msg.DevceID,
				Content:  string(data),
			}

			data, err = json.Marshal(&rawMessage)
			if err != nil {
				log.Println(err)
				continue
			}

			log.Println("Sending message to:", rawMessage.DeviceID)

			socket.Write(append(data, '\n'))
		}
	}(ipcSocket)

	return nil
}

func handleDeviceRequest(w http.ResponseWriter, r *http.Request) {
	r.ParseForm()

	id := r.FormValue("device_id")
	device_id, err := strconv.Atoi(id)
	if err != nil {
		log.Println(err)
		w.WriteHeader(http.StatusBadRequest)
		return
	}

	msg := DeviceMessage{
		DevceID: device_id,
	}

	ipcChan <- msg
	log.Println("Message sent to channel")

	w.Write([]byte("NICE"))
}
