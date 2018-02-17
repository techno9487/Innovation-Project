package main

import (
	"database/sql"
	"log"
	"net/http"

	"github.com/justinas/alice"

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
	router.HandleFunc("/login", handleLogin)

	chain := alice.New(fetchSessionMiddleware, checkAuthenticityMiddleware).ThenFunc(handleDashboard)
	router.Handle("/dashboard", chain)

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

	t.Execute(res, nil)
}
