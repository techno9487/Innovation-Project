package main

import (
	"net/http"

	"encoding/json"

	"context"

	"github.com/google/uuid"
)

const (
	SESSION_STORAGE_KEY = "key:sessionStorage"
)

type SessionStorage map[string]interface{}

type Session struct {
	uid     string
	storage SessionStorage
}

func (sesh *Session) save() error {
	data, err := json.Marshal(&sesh.storage)
	if err != nil {
		return err
	}

	_, err = globalDB.Exec("update Session set data=? where uid = ?", data, sesh.uid)
	if err != nil {
		return err
	}

	return nil
}

func fetchSession(r *http.Request) (Session, error) {
	sesh := Session{}

	cookie, err := r.Cookie("session")
	if err != nil {
		return sesh, err
	}

	sesh.uid = cookie.Value

	row := globalDB.QueryRow("select data from Session where uid = ?", cookie.Value)
	var data string
	err = row.Scan(&data)
	if err != nil {
		return sesh, err
	}

	err = json.Unmarshal([]byte(data), &sesh.storage)
	if err != nil {
		return sesh, err
	}

	return sesh, nil
}

func createSession(w http.ResponseWriter) (Session, error) {
	uid := uuid.New()

	cok := http.Cookie{
		Name:   "session",
		Value:  uid.String(),
		MaxAge: 0,
	}

	http.SetCookie(w, &cok)

	sesh := Session{
		uid:     uid.String(),
		storage: SessionStorage{},
	}

	_, err := globalDB.Exec("insert into Session (uid) values (?)", uid)
	if err != nil {
		return sesh, err
	}

	return sesh, nil
}

func fetchSessionMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {

		sesh, err := fetchSession(r)
		if err != nil {
			next.ServeHTTP(w, r)
		}

		ctx := context.WithValue(r.Context(), SESSION_STORAGE_KEY, sesh)

		next.ServeHTTP(w, r.WithContext(ctx))
	})
}
