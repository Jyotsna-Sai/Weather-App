const apiKey = "YOUR_API_KEY";


async function getWeather() {

    const city =
        document.getElementById("cityInput").value.trim();

    const error =
        document.getElementById("error");


    if (city === "") {

        error.textContent =
            "Please enter a city name.";

        return;
    }


    const url =
        `https://api.openweathermap.org/data/2.5/weather?q=${city}&appid=${apiKey}&units=metric`;


    try {

        error.textContent = "Loading...";


        const response = await fetch(url);


        if (!response.ok) {

            throw new Error(
                "City not found or API error"
            );
        }


        const data = await response.json();


        document.getElementById("city").textContent =
            data.name + ", " + data.sys.country;


        document.getElementById("temperature").textContent =
            data.main.temp;


        document.getElementById("description").textContent =
            data.weather[0].description;


        document.getElementById("feelsLike").textContent =
            data.main.feels_like + " °C";


        document.getElementById("humidity").textContent =
            data.main.humidity + " %";


        document.getElementById("wind").textContent =
            data.wind.speed + " m/s";


        document.getElementById("pressure").textContent =
            data.main.pressure + " hPa";


        error.textContent = "";

    }

    catch (err) {

        error.textContent =
            "Unable to find weather for this city.";

    }
}