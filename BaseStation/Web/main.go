package main

import (
	"database/sql"
	"log"
	"net/http"

	_ "github.com/go-sql-driver/mysql"

	"html/template"

	"github.com/gorilla/mux"
)

var globalDB *sql.DB

func main() {

	initDB()
	defer globalDB.Close()

	router := mux.NewRouter()

	router.HandleFunc("/", homeHandler)

	http.Handle("/", router)
	log.Fatal(http.ListenAndServe("0.0.0.0:8080", nil))
}

func initDB() {
	db, err := sql.Open("mysql", "root:password@/project?parseTime=true")
	if err != nil {
		log.Fatal(err)
	}
	globalDB = db

}

func homeHandler(res http.ResponseWriter, r *http.Request) {
	t, err := template.ParseFiles("views/base.html", "views/index.html")
	if err != nil {
		log.Println(err)
	}

	data := map[string]interface{}{}
	data["names"] = fetchRooms()

	t.Execute(res, data)
}
