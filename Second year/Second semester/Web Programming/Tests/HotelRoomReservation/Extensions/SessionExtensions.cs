using System.Text.Json;
using Microsoft.AspNetCore.Http;

namespace HotelRoomReservation.Extensions
{
    public static class SessionExtensions
    {
        public static void SetObject<T>(this ISession session, string key, T value)
        {
            session.SetString(key, JsonSerializer.Serialize(value));
        }

        public static T? GetObject<T>(this ISession session, string key)
        {
            var value = session.GetString(key);
            return value == null ? default : JsonSerializer.Deserialize<T>(value);
        }

        public static int GetUserId(this ISession session)
        {
            return session.GetInt32("UserId") ?? 0;
        }
    }
}