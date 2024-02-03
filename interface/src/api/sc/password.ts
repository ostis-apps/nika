export function encryptPassword(password: string) {
    let encryptedPassword = '';
    let alphabet = 'abcdefghijklmnopqrstuvwxyz';
    for (let i = 0; i < password.length; i++) {
        encryptedPassword += alphabet[i % alphabet.length] + '2' + password[i];
    }
    return encryptedPassword.split('').reverse().join('');
}

export function decryptPassword(encryptedPassword: string) {
    let decryptedPassword = '';
    let reversedEncryptedPassword = encryptedPassword.split('').reverse().join('');
    for (let i = 0; i < reversedEncryptedPassword.length; i += 3) {
        decryptedPassword += reversedEncryptedPassword[i + 2];
    }
    return decryptedPassword;
}
