package main

import (
	"html/template"
	"log"
	"net/http"
)

type Device struct {
	Id         int
	DeviceName string
}

func handleDashboard(w http.ResponseWriter, r *http.Request) {
	tmp, err := template.ParseFiles("views/base.html", "views/dashboard.html")
	if err != nil {
		log.Println(err)
	}

	ctx := r.Context()
	rawsesh := ctx.Value(SESSION_STORAGE_KEY)
	sesh := rawsesh.(Session)

	id := sesh.storage["uid"]
	row := globalDB.QueryRow("select firstname from User where id = ?", id)

	var name string
	err = row.Scan(&name)
	if err != nil {
		log.Println(err)
	}

	rows, err := globalDB.Query("select Id,DeviceName from Device")
	if err != nil {
		log.Println("Error fetching device list: ", err)
	}
	defer rows.Close()

	devices := make([]Device, 0)
	for rows.Next() {
		var id int
		var name string

		err = rows.Scan(&id, &name)
		if err != nil {
			log.Println(err)
		}

		devices = append(devices, Device{
			Id:         id,
			DeviceName: name,
		})
	}

	data := map[string]interface{}{}
	data["firstname"] = name
	data["devices"] = devices

	tmp.Execute(w, data)
}
