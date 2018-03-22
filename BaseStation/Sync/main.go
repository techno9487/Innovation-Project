// The sync program is the initial connection point for all devices in the smart home
// It handles the transfer of the hub's access point data and the device unique encryption keys
package main

import (
	"crypto/rand"
	"log"
)

// Main entry point for the sync program
func main() {
	log.Println("Starting Sync daemon...")
}

// Generates a cryptographically random Key
func generateKey(keySize int) ([]byte, error) {
	buffer := make([]byte, keySize)
	_, err := rand.Read(buffer)
	if err != nil {
		return buffer, err
	}
	return buffer, nil
}
