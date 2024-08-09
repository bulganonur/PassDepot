## Future Plans/Road Map

### Must-Haves

1. Virtual memory lock for anything and everything plaintext
    - DepotUsername
    - Masterpassword
    - MasterpassVerify

    - Title
    - Username
    - Password
    - Notes
    - UserId

    - Key

    - Display Depot
    
    - Local/Temporary variables:
        - Plaintext used for decryption
        - Buffer struct used for making plaintext pretty
        - Buffer string used for concatenating user input

2. Encrypt DepotUsername
    - We already have a key derived from the user's masterpassword. It is used for encrypting entries.

3. Everything stays encrypted in memory and only decrypt entries at user's will
    - Showing everything encrypted is not user friendly at all. It will be almost impossible for users to distinguish 
    entries. Title and EntryId should shown as plaintext and the rest can be shown as ciphertext. Also consider 
    performance overhead. With the current setup it may not be possible to decrypt parts of an entry.

4. Encrypt database file ???

### Nice-to-Haves

- Make table sortable
- Make theme transparecy adjustable
- Password generation
- Let users create additional tables/depots/vaults and or additional databases.
- Switch to Dear Imgui's stdlib for C++ stl
- Libsodium padding? -> [read](https://doc.libsodium.org/padding)
- Salt entry passwords?
- Encrypt EntryInfo?
- A Tips window for the quirks of PassDepot:
    - Holding LShift disables window dragging
- Store entry/depot creation dates, last modified date in database
- Implementing tooltips for some items would be nice
- Users should be able to change their masterpassword
- Maximized window should zero window rounding
- Maybe change some of the database queries with DisplayDepot queries