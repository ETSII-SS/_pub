# Set-ExecutionPolicy -ExecutionPolicy Unrestricted -Scope CurrentUser
# Variables globales de uso común
$___parentDir = Split-Path -Parent -Path $PSScriptRoot
$global:ss = New-Object PSObject -Property @{
    ps1Root= $___parentDir
    ssRoot= Split-Path -Parent -Path $___parentDir
    #Fallo IA: pubDir = "$ssRoot\_pub"
    pubDir = ""
    pubUrl = "https://github.eii.us.es/IC3-SS/_pub.git"
    #privUrl = "https://github.eii.us.es/IC3-SS/"
    #pubUrl = "https://github.eii.us.es/IC3-SS/_pub_priv.git"
    privUrl = "https://github.eii.us.es/IC3-SS/"
    privDir = ""
    toutFin = 10
    #Fallo IA: AACommEsperado= "$pubDir\_comm\"
    AACommEsperado= ""
}



# Funciones

## Comprueba que los workspaces están bien configurados
function CompruebaWokspaces {
 
    $nombreUsuario = PideUvus
    if ($nombreUsuario -eq $null) {
        return $false
    } else {
        Write-Host "El UVUS proporcionado fue: $nombreUsuario"
    }

    $ss.pubDir = "$($ss.ssRoot)\_pub"
    $ss.AACommEsperado= "$($ss.pubDir)\_comm\"


    ## Comprueba que la estructura de directorios es correcta
    Write-Host "El directorio SSRoot está en $($ss.ssRoot)" 

    # Los directorios existen
    if (-not (ComprobarDirectorios @($($ss.pubDir), $($ss.privDir)))){
        return $false
    }

    # Repositorio público y privado existen
    if (-not (EsWorkspaceGit $($ss.pubDir) $($ss.pubUrl))){
        return $false
    }
    if (-not (EsWorkspaceGit $($ss.privDir) $($ss.privUrl))){
        return $false
    }

    # Repositorio público y privado están actualizados
    if (-not (ComprobarUltimaVersionGit $($ss.pubDir) $($ss.pubUrl))){
        return $false
    }
    # permite que el directorio privado no esté actualizado. 
    if (-not (ComprobarUltimaVersionGit $($ss.privDir) $($ss.privUrl))){
        ## No hace nada por ahora. 
    }

    #Variable de entorno AAComm tiene valor correcto
    if (-not ($env:AAComm -eq $($ss.AACommEsperado))) {
        EscribeError "La variable de entorno AAComm no tiene el valor correcto"
        EscribeError "              Vale: $env:AAComm"
        EscribeError "     Debería valer: $($ss.AACommEsperado)"
    }
    
    return $true
}




# Pide uvus y ajusta URL privados
function PideUvus {
    $uvus = Read-Host -Prompt 'Por favor, introduce tu UVUS'
    if ($uvus.Length -lt 3) {
        EscribeError "Error: UVUS no válido. Debe tener al menos 3 letras."
        return $null
    } else {
        $ss.privUrl += "$uvus.git"
        $ss.privDir = "$($ss.ssRoot)\$uvus"
        return $uvus
    }
}

function EscribeError{
    param (
        [Parameter(Mandatory=$true)]
        [string]$mensaje
    )

    Write-Host "ERROR: $mensaje" -ForegroundColor Red
}

function ComprobarDirectorios {
    param (
        [Parameter(Mandatory=$true)]
        [string[]]$directorios
    )

    $todosExisten = $true

    foreach ($directorio in $directorios) {
        if (-not (Test-Path $directorio)) {
            EscribeError "El directorio $directorio no existe." 
            $todosExisten = $false
        }
    }

    return $todosExisten
}

function EsWorkspaceGit {
    param (
        [Parameter(Mandatory=$true)]
        [string]$directorio,
        [Parameter(Mandatory=$true)]
        [string]$repositorioGit
    )

    Set-Location -Path $directorio

    try {
        $remoteUrl = git remote -v | Out-String
    } catch {
        EscribeError "Error: El directorio $directorio no es un workspace de Git." 
        return $false
    }

    if ($remoteUrl -like "*$repositorioGit*") {
        #Write-Host "El directorio $directorio es un workspace del repositorio Git $repositorioGit."
        return $true
    } else {
        EscribeError "El directorio $directorio no es un workspace del repositorio Git $repositorioGit."
        return $false
    }
}


#  5 versiones. al final he tenido que reescribir baste 
function ComprobarUltimaVersionGit {
    param (
        [Parameter(Mandatory=$true)]
        [string]$directorio,
        [Parameter(Mandatory=$true)]
        [string]$repositorioGit
    )
    try {
        Set-Location -Path $directorio

        $localHash = git rev-parse HEAD
        $salida = & git ls-remote $repositorioGit HEAD
        $partes = $salida -split "\s"
        $remoteHash = $partes[0]
        if ($localHash -eq $remoteHash) {
            return $true
        } else {
            EscribeError "El directorio $directorio no tiene la última versión del repositorio Git $repositorioGit.`n     Actualice el directorio o repositorio"
            return $false
        }
    } catch {
        EscribeError "No se pudo comprobar si el directorio $directorio tiene la última versión del repositorio GIT $repositorioGit" 
        return $false
    }

    $scriptblock = {
        param($directorio, $repositorioGit)

        Set-Location -Path $directorio

        $localHash = git rev-parse HEAD
        $remoteHash = git ls-remote $repositorioGit -h refs/heads/master | %{ $_.Split("\t")[0] }

        if ($localHash -eq $remoteHash) {
            return $true
        } else {
            EscribeError "El directorio $directorio no tiene la última versión del repositorio Git $repositorioGit."
            return $false
        }
    }

    Start-Process -FilePath "powershell.exe" -ArgumentList "-Command & {$scriptblock} '$directorio' '$repositorioGit'"
}

# no conseguido: la intención era emular el comando timeout de msdos: espera n segundos o hasta que el usuario pulse una tecla.
function Termina {
   param (
        [bool]$resTest = $false
    )
    if ($resTest) {
        Write-Output "    ¡TEST CORRECTO"
    }

    Read-Host "Pulse ENTER para terminar"

}
