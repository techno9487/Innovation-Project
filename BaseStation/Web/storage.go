package main

import (
	"log"
)

func fetchRooms() []string {
	rooms := make([]string, 0)

	rows, err := globalDB.Query("select RoomName from Room")
	if err != nil {
		log.Println(err)
	}

	defer rows.Close()

	for rows.Next() {
		var name string
		err = rows.Scan(&name)
		if err != nil {
			log.Println(err)
			break
		}
		rooms = append(rooms, name)
	}

	return rooms
}

type Log struct {
	Info       string
	DeviceName string
}
