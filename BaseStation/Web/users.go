package main

import (
	"log"
	"net/http"

	"encoding/json"

	"html/template"

	"golang.org/x/crypto/bcrypt"
)

func changePasswordMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		sesh := r.Context().Value(SESSION_STORAGE_KEY).(Session)

		id := sesh.storage["uid"]
		if id == nil {
			next.ServeHTTP(w, r)
		}

		row := globalDB.QueryRow("select (UserMetadata) from User where Id = ?", id.(float64))
		var data string

		row.Scan(&data)

		if len(data) == 0 {
			next.ServeHTTP(w, r)
			return
		}

		storage := map[string]interface{}{}

		err := json.Unmarshal([]byte(data), &storage)
		if err != nil {
			log.Println(err)
		}

		if storage["passwordChange"] == true {
			http.Redirect(w, r, "/passwordChange", 301)
			return
		}

		next.ServeHTTP(w, r)
	})
}

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

	row := globalDB.QueryRow("select Id,Password from User where email = ?", email)
	var password string
	var Id int
	err := row.Scan(&Id, &password)
	if err != nil {
		log.Println("Login scan err:", err)
		w.Write([]byte("Database error"))
		return
	}

	err = bcrypt.CompareHashAndPassword([]byte(password), []byte(r.FormValue("password")))
	if err != nil {
		//log.Println(err)
		w.Write([]byte("Wrong username or password"))
		return
	}

	sesh, err := createSession(w)
	if err != nil {
		log.Println("Creating sesh:", err)
		return
	}

	sesh.storage["loggedIn"] = true
	sesh.storage["uid"] = Id

	err = sesh.save()
	if err != nil {
		log.Println("Saving sesh:", err)
	}

	http.Redirect(w, r, "/dashboard", 301)
}

func handlePasswordChange(w http.ResponseWriter, r *http.Request) {

	tmp, err := template.ParseFiles("views/base.html", "views/passwordChange.html")
	if err != nil {
		log.Println(err)
	}
	tmp.Execute(w, nil)
}

func handlePasswordChangeBackend(w http.ResponseWriter, r *http.Request) {
	r.ParseForm()

	tmp, err := template.ParseFiles("views/base.html", "views/passwordChange.html")
	if err != nil {
		log.Println(err)
	}

	//Is either of the password fields blank
	if len(r.FormValue("newPassword1")) < 1 {
		tmp.Execute(w, map[string]interface{}{
			"failureMessage": "Blank password",
		})
		return
	}

	//checks if the password fields match
	if r.FormValue("newPassword1") != r.FormValue("newPassword2") {

		tmp.Execute(w, map[string]interface{}{
			"failureMessage": "Passwords don't match",
		})
		return
	}

	//check if the old password matches
	sesh := r.Context().Value(SESSION_STORAGE_KEY).(Session)
	row := globalDB.QueryRow("select (Password) from User where Id = ?", sesh.storage["uid"])
	var pwd string

	err = row.Scan(&pwd)
	if err != nil {
		log.Println("Password Change Err:", err)
	}

	err = bcrypt.CompareHashAndPassword([]byte(pwd), []byte(r.FormValue("oldPassword")))
	if err != nil {
		tmp.Execute(w, map[string]interface{}{
			"failureMessage": "old password doesn't match",
		})
		return
	}

	data, err := bcrypt.GenerateFromPassword([]byte(r.FormValue("newPassword1")), bcrypt.DefaultCost)
	if err != nil {
		tmp.Execute(w, map[string]interface{}{
			"failureMessage": err.Error(),
		})
		return
	}

	result, err := globalDB.Exec("update User set Password = ? where Id = ?", data, sesh.storage["uid"])
	if err != nil {
		log.Println(err)
		return
	}
	log.Println(result.RowsAffected())

	http.Redirect(w, r, "/dashboard", 301)
}
