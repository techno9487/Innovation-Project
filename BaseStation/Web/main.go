package main

import (
	"log"
	"net/http"

	"html/template"

	"github.com/gorilla/mux"
)

func main() {

	router := mux.NewRouter()

	router.HandleFunc("/", homeHandler)

	http.Handle("/", router)
	log.Fatal(http.ListenAndServe("0.0.0.0:8080", nil))
}

func homeHandler(res http.ResponseWriter, r *http.Request) {
	t, err := template.ParseFiles("views/index.html")
	if err != nil {
		log.Println(err)
	}

	t.Execute(res, nil)
}
