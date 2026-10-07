const logEl = document.querySelector("pre");

function log(msg, status = "info") {
  const now = new Date();
  const timestamp = now.toLocaleString().replace(", ", " ");

  const colors = {
    info: "var(--color)",
    warning: "var(--warning-color)",
    success: "var(--success-color)",
    error: "var(--error-color)",
  };

  logEl.innerHTML += `[${timestamp}] <span style="color: ${colors[status] || colors.info}">${msg}</span>\n`;
  logEl.scrollTop = logEl.scrollHeight;
}

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

function fetchWithTimeout(url, ms = 500, options = {}) {
  if (ms <= 0) return fetch(url);
  const controller = new AbortController();
  const timer = setTimeout(() => controller.abort(), ms);
  const fetchOptions = { ...options, signal: controller.signal };
  return fetch(url, fetchOptions).finally(() => clearTimeout(timer));
}

async function checkFileExists(fileUrl) {
  const fileName = fileUrl.split("/").pop();
  try {
    const response = await fetchWithTimeout(fileUrl, 500, {
      method: "GET",
    });
    const contentType = response.headers.get("Content-Type");
    if (
      response.ok &&
      contentType &&
      contentType.includes("application/octet-stream")
    ) {
      log(`File exists: ${fileName}`);
      return true;
    } else {
      log(`File not found: ${fileName}`, "error");
      return false;
    }
  } catch (error) {
    log(`Access error: ${error.message}`, "error");
    return false;
  }
}
