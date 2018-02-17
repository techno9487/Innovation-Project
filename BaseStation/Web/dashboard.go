package main

import (
	"html/template"
	"log"
	"net/http"
)

func handleDashboard(w http.ResponseWriter, r *http.Request) {
	tmp, err := template.ParseFiles("views/base.html", "views/dashboard.html")
	if err != nil {
		log.Println(err)
	}

	tmp.Execute(w, nil)
}
