# 📋 INSTRUCCIONES: IMPORTAR CONFIGURACIÓN DE BETTER BIBTEX

## ⚙️ OPCIÓN 1: IMPORTAR CONFIGURACIÓN COMPLETA (RECOMENDADO)

### Pasos:
1. Abre **Zotero**
2. Ve a **Edit → Preferences → Better BibTeX**
3. Haz clic en la pestaña **"Export"**
4. En la parte inferior, busca el botón **"Import"** o **"Import preferences"**
5. Selecciona el archivo: `BetterBibTeX_Config.json`
6. Haz clic en **"Open"** o **"Abrir"**
7. **Reinicia Zotero** para aplicar los cambios

---

## ⚙️ OPCIÓN 2: CONFIGURAR MANUALMENTE SOLO CITATION KEYS

Si solo quieres cambiar la fórmula de citation keys:

### Pasos:
1. Abre **Zotero**
2. Ve a **Edit → Preferences → Better BibTeX → Citation Keys**
3. En el campo **"Citation key format"**, pega esto:

```
[auth:lower][year][veryshorttitle:lower]
```

4. Haz clic en **OK**

---

## 🔄 REGENERAR CITATION KEYS EXISTENTES (IMPORTANTE)

Después de cambiar la fórmula, debes regenerar las claves de las referencias existentes:

### Pasos:
1. En Zotero, selecciona **todas las referencias** de tu biblioteca (Ctrl+A)
2. **Click derecho** → **Better BibTeX → Refresh BibTeX key**
3. Espera a que se actualicen todas las claves
4. Verifica que se actualizaron correctamente

---

## ✅ VERIFICAR CONFIGURACIÓN

Después de importar, verifica:

1. **Citation key format:**
   - Edit → Preferences → Better BibTeX → Citation Keys
   - Debe mostrar: `[auth:lower][year][veryshorttitle:lower]`

2. **Auto-export configurado:**
   - Edit → Preferences → Better BibTeX → Automatic Export
   - Debe aparecer tu exportación a `Referencias.bib`

3. **Ejemplos de citation keys:**
   - Selecciona una referencia
   - En el panel derecho, busca el campo "Citation Key"
   - Debe verse como: `smith2024smarthome` o `garcia2023iot`

---

## 📊 FÓRMULA DE CITATION KEYS EXPLICADA

### Formato: `[auth:lower][year][veryshorttitle:lower]`

**Ejemplos:**
- **Autor:** Smith | **Año:** 2024 | **Título:** "Smart Home IoT Architecture"
  - **Citation key:** `smith2024smart`

- **Autor:** García | **Año:** 2023 | **Título:** "IEEE 802.11ah Performance"
  - **Citation key:** `garcia2023ieee`

- **Autor:** Kumar et al. | **Año:** 2025 | **Título:** "Edge Computing Gateway"
  - **Citation key:** `kumar2025edge`

**Componentes:**
- `[auth:lower]` = Primer autor en minúsculas (e.g., "smith", "garcia")
- `[year]` = Año de publicación (e.g., "2024", "2023")
- `[veryshorttitle:lower]` = Primera palabra significativa del título en minúsculas (e.g., "smart", "ieee", "edge")

---

## 🚀 SIGUIENTE PASO

Después de importar la configuración:
1. Regenera las citation keys de tus referencias existentes
2. Fuerza la exportación: Edit → Preferences → Better BibTeX → Automatic Export → "Export now"
3. Verifica que `Referencias.bib` se actualizó con las nuevas claves

---

## ❓ TROUBLESHOOTING

**Problema:** No encuentro el botón "Import" en Better BibTeX
- **Solución:** Algunas versiones usan "Restore preferences from file..." en la parte inferior de la pestaña Export

**Problema:** Las citation keys no cambiaron después de importar
- **Solución:** Debes seleccionar todas las referencias y hacer "Refresh BibTeX key" manualmente

**Problema:** Duplicados de citation keys
- **Solución:** Better BibTeX añadirá sufijos automáticamente (e.g., `smith2024smart`, `smith2024smarta`, `smith2024smartb`)
