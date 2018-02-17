package main

import (
	"log"
	"net/http"

	"golang.org/x/crypto/bcrypt"
)

func checkAuthenticityMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		ctx := r.Context()
		data := ctx.Value(SESSION_STORAGE_KEY)

		if data == nil {
			http.Redirect(w, r, "/", 301)
			return
		}

		sesh := data.(Session)
		if sesh.storage["loggedIn"] != true {
			http.Redirect(w, r, "/", 301)
			return
		}

		next.ServeHTTP(w, r)
	})
}

func handleLogin(w http.ResponseWriter, r *http.Request) {
	r.ParseForm()

	email := r.FormValue("email")

	row := globalDB.QueryRow("select (Password) from User where email = ?", email)
	var password string
	err := row.Scan(&password)
	if err != nil {
		log.Println(err)
		w.Write([]byte("Database error"))
		return
	}

	err = bcrypt.CompareHashAndPassword([]byte(password), []byte(r.FormValue("password")))
	if err != nil {
		log.Println(err)
		w.Write([]byte("Wrong username or password"))
		return
	}

	sesh, err := createSession(w)
	if err != nil {
		log.Println("Creating sesh:", err)
		return
	}

	sesh.storage["loggedIn"] = true
	err = sesh.save()
	if err != nil {
		log.Println("Saving sesh:", err)
	}

	http.Redirect(w, r, "/dashboard", 301)
}
